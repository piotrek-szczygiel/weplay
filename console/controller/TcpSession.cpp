#include "TcpSession.hpp"
#include <boost/log/trivial.hpp>

namespace ba = boost::asio;

void TcpSession::start()
{
    BOOST_LOG_TRIVIAL(info) << "Started new session with "
                            << m_socket.remote_endpoint().address().to_string();

    do_read();
}

void TcpSession::do_read()
{
    auto self { shared_from_this() };
    m_socket.async_read_some(ba::buffer(m_data, MAX_LENGTH),
        [this, self](boost::system::error_code ec, std::size_t length) {
            auto address = m_socket.remote_endpoint().address().to_string();

            if (ec) {
                BOOST_LOG_TRIVIAL(error)
                    << "Error while receiving data from " << address << ": " << ec.message();
                return;
            }

            BOOST_LOG_TRIVIAL(info) << "Received " << length << " bytes from " << address;

            if (!m_valid_controller && length == 4) {
                if (m_data[0] == 0x19 && m_data[1] == 0x84 && m_data[2] == 0x01
                    && m_data[3] == '\n') {
                    m_valid_controller = true;
                }
            } else if (m_valid_controller && length == 4) {
                if (m_data[0] == 'b' && m_data[3] == '\n') {
                    bool key_down {};

                    if (m_data[2] == 'd') {
                        key_down = true;
                    } else if (m_data[2] == 'u') {
                        key_down = false;
                    } else {
                        BOOST_LOG_TRIVIAL(error) << "Invalid EventKey.down: " << m_data[2];
                        return;
                    }

                    switch (m_data[1]) {
                    case '<':
                        m_controller_state->left.store(key_down);
                        break;
                    case '^':
                        m_controller_state->forward.store(key_down);
                        break;
                    case '>':
                        m_controller_state->right.store(key_down);
                        break;
                    default:
                        BOOST_LOG_TRIVIAL(error) << "Invalid EventKey.key: " << m_data[1];
                        return;
                    }
                }
            }

            do_read();
        });
}
