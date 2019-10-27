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
    m_socket.async_read_some(ba::buffer(m_data, m_read_size),
        [this, self](boost::system::error_code ec, std::size_t length) {
            auto address = m_socket.remote_endpoint().address().to_string();

            if (ec) {
                BOOST_LOG_TRIVIAL(error)
                    << "Error while receiving data from " << address << ": " << ec.message();
                return;
            }

            BOOST_LOG_TRIVIAL(info) << "Received " << length << " bytes from " << address;

            if (length != m_read_size) {
                BOOST_LOG_TRIVIAL(warning) << "ERROR: expected " << m_read_size << " bytes";
                return;
            }

            if (!m_valid_controller) {
                if (m_data[0] == '%') {
                    m_valid_controller = true;
                    BOOST_LOG_TRIVIAL(info) << "Controller successfully registered";
                } else {
                    BOOST_LOG_TRIVIAL(warning) << "Invalid data received";
                    return;
                }
            } else if (m_next_read == NextRead::None) {
                if (m_data[0] == 'B') {
                    m_next_read = NextRead::Buttons;
                    m_read_size = 1;
                } else if (m_data[0] == 'G') {
                    m_next_read = NextRead::Mpu6050;
                    m_read_size = 6;
                }
            } else {
                if (m_next_read == NextRead::Buttons) {
                    for (uint8_t i = 0; i < 8; ++i) {
                        m_controller_state->buttons[i] = !(m_data[1] & (1 << i));
                    }
                } else if (m_next_read == NextRead::Mpu6050) {
                    m_controller_state->yaw = static_cast<int16_t>(m_data[1]);
                    m_controller_state->yaw |= static_cast<int16_t>(m_data[2]) << 8;

                    m_controller_state->pitch = static_cast<int16_t>(m_data[3]);
                    m_controller_state->pitch |= static_cast<int16_t>(m_data[4]) << 8;

                    m_controller_state->roll = static_cast<int16_t>(m_data[5]);
                    m_controller_state->roll |= static_cast<int16_t>(m_data[6]) << 8;
                }

                m_next_read = NextRead::None;
                m_read_size = 1;
            }

            do_read();
        });
}
