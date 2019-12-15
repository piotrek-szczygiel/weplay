#include "Connection.hpp"
#include "Print.hpp"

void Connection::connect()
{
    if (WiFi.status() != WL_CONNECTED) {
        connect_wifi();
    }

    if (!m_discovered) {
        discover_console_ip();
    }

    if (!m_connected) {
        connect_server();
    }
}

void Connection::connect_wifi()
{
    print("Connecting to %s", m_ssid);
    WiFi.begin(m_ssid, m_password);

    while (WiFi.status() != WL_CONNECTED) {
        print(".");
        delay(500);
    }

    println();
    println("Connected to %s", m_ssid);
}

void Connection::discover_console_ip()
{
    print("Discovering console on udp port %u", m_port);
    m_udp.begin(m_port);

    while (!m_discovered) {
        print(".");
        delay(500);

        if (WiFi.status() != WL_CONNECTED) {
            println();
            connect_wifi();
        }

        int len = m_udp.parsePacket();
        if (len > 0) {
            len = m_udp.read(m_udp_packet, MAX_UDP_PACKET_SIZE);

            if (len > 0 && len < MAX_UDP_PACKET_SIZE) {
                m_udp_packet[static_cast<size_t>(len)] = 0;

                if (strcmp("raspberry-console", m_udp_packet) == 0) {
                    m_ip = m_udp.remoteIP();
                    m_discovered = true;
                    break;
                } else {
                    println();
                    println("Unknown packet received: %s", m_udp_packet);
                }
            } else {
                println();
                println("Received UDP packet with invalid size: %d", len);
            }
        }
    }

    println();
    println("Found console at %s", m_ip.toString().c_str());
}

void Connection::connect_server()
{
    m_udp.beginPacket(m_ip, m_port);
    m_udp.write("ping");
    m_udp.endPacket();

    print("Connecting to %s:%u", m_ip.toString().c_str(), m_port);
    while (!m_connected) {
        print(".");
        delay(500);

        if (WiFi.status() != WL_CONNECTED) {
            println();
            connect_wifi();
        }

        int len = m_udp.parsePacket();
        if (len > 0) {
            len = m_udp.read(m_udp_packet, MAX_UDP_PACKET_SIZE);

            if (len > 0 && len < MAX_UDP_PACKET_SIZE) {
                m_udp_packet[static_cast<size_t>(len)] = 0;
                if (strcmp("pong", m_udp_packet) == 0) {
                    m_connected = true;
                    m_ip = m_udp.remoteIP();
                    break;
                }
            }
        }
    }

    println();
    println("Connected to %s:%u", m_ip.toString().c_str(), m_port);
    m_ping = 0;
}

void Connection::ping()
{
    if (m_connected) {
        m_udp.beginPacket(m_ip, m_port);
        m_udp.write("ping");
        m_udp.endPacket();
        ++m_ping;

        if (m_ping >= 5) {
            println("Connection with console timed out!");
            m_discovered = false;
            m_connected = false;
        }
    }
}

void Connection::pong()
{
    if (m_connected) {
        int len = m_udp.parsePacket();
        if (len > 0) {
            len = m_udp.read(m_udp_packet, MAX_UDP_PACKET_SIZE);
            if (len > 0 && len < MAX_UDP_PACKET_SIZE) {
                m_udp_packet[static_cast<size_t>(len)] = 0;
                if (strcmp("pong", m_udp_packet) == 0) {
                    m_ping = 0;
                }
            }
        }
    }
}

void Connection::update(uint16_t buttons_state, int16_t yaw, int16_t pitch, int16_t roll)
{
    uint8_t packet[] {
        static_cast<uint8_t>((buttons_state & 0x00ff)),
        static_cast<uint8_t>((buttons_state & 0xff00) >> 8),
        static_cast<uint8_t>((yaw & 0x00ff)),
        static_cast<uint8_t>((yaw & 0xff00) >> 8),
        static_cast<uint8_t>((pitch & 0x00ff)),
        static_cast<uint8_t>((pitch & 0xff00) >> 8),
        static_cast<uint8_t>((roll & 0x00ff)),
        static_cast<uint8_t>((roll & 0xff00) >> 8),
    };

    m_udp.beginPacket(m_ip, m_port);
    m_udp.write(packet, sizeof(packet));
    m_udp.endPacket();
}
