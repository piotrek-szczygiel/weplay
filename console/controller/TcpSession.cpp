#include "TcpSession.hpp"
#include <boost/log/trivial.hpp>

namespace ba = boost::asio;

void TcpSession::start()
{
    BOOST_LOG_TRIVIAL(info) << "Started new session with " << socket_.remote_endpoint().address().to_string();

    doRead();
}

void TcpSession::doRead()
{
    auto self { shared_from_this() };
    socket_.async_read_some(
        ba::buffer(data_, max_length_), [this, self](boost::system::error_code ec, std::size_t length) {
            auto address = socket_.remote_endpoint().address().to_string();

            if (ec) {
                BOOST_LOG_TRIVIAL(error) << "Error while receiving data from " << address << ": " << ec.message();
                return;
            }

            BOOST_LOG_TRIVIAL(info) << "Received " << length << " bytes from " << address;

            if (!validController_ && length == 4) {
                if (data_[0] == 0x19 && data_[1] == 0x84 && data_[2] == 0x01 && data_[3] == '\n') {

                    validController_ = true;
                }
            } else if (validController_ && length == 4) {
                if (data_[0] == 'b' && data_[3] == '\n') {
                    bool key_down {};

                    if (data_[2] == 'd') {
                        key_down = true;
                    } else if (data_[2] == 'u') {
                        key_down = false;
                    } else {
                        BOOST_LOG_TRIVIAL(error) << "Invalid EventKey.down: " << data_[2];
                        return;
                    }

                    switch (data_[1]) {
                    case '<':
                        controllerState_->left.store(key_down);
                        break;
                    case '^':
                        controllerState_->forward.store(key_down);
                        break;
                    case '>':
                        controllerState_->right.store(key_down);
                        break;
                    default:
                        BOOST_LOG_TRIVIAL(error) << "Invalid EventKey.key: " << data_[1];
                        return;
                    }
                }
            }

            doRead();
        });
}
