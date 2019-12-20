#include <string>
#include <vector>

#ifdef _WIN32
#include "../Windows.hpp"
#else
#include <netinet/in.h>
#define INVALID_SOCKET -1
#define SOCKET int
#endif

struct ReceiveResult {
    bool success;
    bool timedout;
    sockaddr_in addr;
    std::string repr;

    uint8_t* data;
    int size;
};

struct LocalAddress {
    uint32_t address;
    uint32_t mask;
};

std::string last_error();
std::string int_to_ip(uint32_t);
std::vector<LocalAddress> get_local_addresses();

class BroadcastSocket {
public:
    BroadcastSocket(LocalAddress address, uint16_t port)
        : m_local_address { address }
        , m_port { port }
    {
        m_address = m_local_address.address | (~m_local_address.mask);
    }

    std::string info() const;
    bool initialize();
    bool send(const std::string& data);

private:
    LocalAddress m_local_address;

    uint32_t m_address;
    uint16_t m_port;

    SOCKET m_socket {};
    sockaddr_in m_addr {};
};

class ServerSocket {
public:
    explicit ServerSocket(uint16_t port)
        : m_port { port }
    {
    }

    bool bind();

    ReceiveResult receive();
    bool send(sockaddr_in addr, const std::string& data);

private:
    SOCKET m_socket {};
    uint16_t m_port;

    sockaddr_in m_addr {};
};
