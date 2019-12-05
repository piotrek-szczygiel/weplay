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

    if (!m_client.connected()) {
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
    println("Discovering console on udp port %u...", m_port);
    m_udp.begin(m_port);

    while (true) {
        int size = m_udp.parsePacket();
        if (size > 0) {
            int len = m_udp.read(m_udp_packet, MAX_UDP_PACKET_SIZE);

            if (len > 0 && len < MAX_UDP_PACKET_SIZE) {
                m_udp_packet[static_cast<size_t>(len)] = 0;

                if (strcmp("raspberry-console", m_udp_packet) == 0) {
                    println("Found console at %s", m_ip.toString().c_str());
                    m_ip = m_udp.remoteIP();
                    m_discovered = true;
                    break;
                } else {
                    println("Unknown packet received: %s", m_udp_packet);
                }
            } else {
                println("Received UDP packet with invalid size: %d", len);
            }
        }
    }
}

void Connection::connect_server()
{
    delay(1000);
    print("Connecting to %s:%u", m_ip.toString().c_str(), m_port);
    while (!m_client.connect(m_ip, m_port)) {
        print(".");
        delay(500);

        if (WiFi.status() != WL_CONNECTED) {
            println();
            connect_wifi();
        }
    }

    println();
    println("Connected to %s:%u", m_ip.toString().c_str(), m_port);
    m_client.setNoDelay(true);
    m_client.write('%');
}

void Connection::send_buttons(uint16_t buttons_state)
{
    uint8_t packet[] {
        'B',
        static_cast<uint8_t>((buttons_state & 0x00ff)),
        static_cast<uint8_t>((buttons_state & 0xff00) >> 8),
    };

    m_client.write(packet, sizeof(packet));
}

void Connection::ping()
{
    uint8_t packet[] { 'P' };
    m_client.write(packet, sizeof(packet));
}

void Connection::send_ypr(int16_t yaw, int16_t pitch, int16_t roll)
{
    uint8_t packet[] {
        'G',
        static_cast<uint8_t>((yaw & 0x00ff)),
        static_cast<uint8_t>((yaw & 0xff00) >> 8),
        static_cast<uint8_t>((pitch & 0x00ff)),
        static_cast<uint8_t>((pitch & 0xff00) >> 8),
        static_cast<uint8_t>((roll & 0x00ff)),
        static_cast<uint8_t>((roll & 0xff00) >> 8),
    };

    m_client.write(packet, sizeof(packet));
}
