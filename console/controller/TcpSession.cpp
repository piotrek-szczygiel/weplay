#include "TcpSession.hpp"
#include <boost/log/trivial.hpp>

namespace ba = boost::asio;

void TcpSession::start()
{
    m_deadline.async_wait(std::bind(&TcpSession::check_deadline, this));
    m_deadline.expires_after(std::chrono::seconds(5));

    read();
}

void TcpSession::check_deadline()
{
    if (m_closed) {
        return;
    }

    if (m_deadline.expiry() <= ba::steady_timer::clock_type::now()) {
        m_socket.close();
        m_deadline.cancel();
    } else {
        m_deadline.async_wait(std::bind(&TcpSession::check_deadline, this));
    }
}

bool TcpSession::register_controller()
{
    if (m_state->connected_num >= MAX_CONTROLLERS) {
        BOOST_LOG_TRIVIAL(warning)
            << "Maximum connected controllers limit reached: " << MAX_CONTROLLERS;
        return false;
    }

    m_valid_controller = true;
    for (m_id = 0; m_id < MAX_CONTROLLERS; ++m_id) {
        if (!m_state->connected[m_id]) {
            break;
        }
    }

    if (m_id >= MAX_CONTROLLERS) {
        BOOST_LOG_TRIVIAL(error)
            << "Unable to obtain controller id, even though it should be possible";
        return false;
    }

    m_state->connected_num += 1;
    m_state->connected[m_id] = true;

    BOOST_LOG_TRIVIAL(info) << "New controller registered with id " << m_id;
    return true;
}

void TcpSession::unregister_controller()
{
    if (m_valid_controller) {
        m_valid_controller = false;
        BOOST_LOG_TRIVIAL(info) << "Unregistering controller with id " << m_id;
        m_state->connected_num -= 1;
        m_state->connected[m_id] = false;
    }

    m_socket.close();
    m_deadline.cancel();
    m_closed = true;
}

void TcpSession::read()
{
    if (m_closed) {
        return;
    }

    auto self { shared_from_this() };

    m_socket.async_read_some(ba::buffer(m_data, m_read_size),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (m_closed) {
                return;
            }

            if (ec) {
                if (ec.value() == 1236) {
                    BOOST_LOG_TRIVIAL(error) << "Controller " << m_id << " timed out";
                } else {
                    BOOST_LOG_TRIVIAL(error) << "Unable to read from controller: " << ec.message();
                }

                return unregister_controller();
            }

            m_deadline.expires_after(std::chrono::seconds(10));

            auto address = m_socket.remote_endpoint().address().to_string();

            if (length != m_read_size) {
                BOOST_LOG_TRIVIAL(error) << "Expected " << m_read_size << " bytes";
                return unregister_controller();
            }

            if (!m_valid_controller) {
                if (m_data[0] == '%') {
                    if (register_controller()) {
                        BOOST_LOG_TRIVIAL(info)
                            << "Controller successfully registered on " << address;
                    } else {
                        return unregister_controller();
                    }
                } else {
                    BOOST_LOG_TRIVIAL(error) << "Unexpected data received from " << address;
                    return unregister_controller();
                }
            } else if (m_next_read == NextRead::Mode) {
                if (m_data[0] == 'B') {
                    m_next_read = NextRead::Buttons;
                    m_read_size = 2;
                } else if (m_data[0] == 'G') {
                    m_next_read = NextRead::Mpu6050;
                    m_read_size = 6;
                } else if (m_data[0] == 'P') { // Ping
                    m_next_read = NextRead::Mode;
                    m_read_size = 1;
                } else {
                    BOOST_LOG_TRIVIAL(error) << "Invalid reading mode received from " << address;
                    return unregister_controller();
                }
            } else {
                if (m_next_read == NextRead::Buttons) {
                    for (uint8_t i = 0; i < 16; ++i) {
                        uint8_t byte = i / 8;
                        uint8_t bit = i % 8;

                        m_state->controllers[m_id].buttons[i] = !(m_data[byte] & (1 << bit));
                    }
                } else if (m_next_read == NextRead::Mpu6050) {
                    m_state->controllers[m_id].yaw = static_cast<int16_t>(m_data[0]);
                    m_state->controllers[m_id].yaw |= static_cast<int16_t>(m_data[1]) << 8;

                    m_state->controllers[m_id].pitch = static_cast<int16_t>(m_data[2]);
                    m_state->controllers[m_id].pitch |= static_cast<int16_t>(m_data[3]) << 8;

                    m_state->controllers[m_id].roll = static_cast<int16_t>(m_data[4]);
                    m_state->controllers[m_id].roll |= static_cast<int16_t>(m_data[5]) << 8;
                }

                m_next_read = NextRead::Mode;
                m_read_size = 1;
            }

            read();
        });
}
