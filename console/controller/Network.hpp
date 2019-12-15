#include <string>
#include <vector>

#ifdef linux
#include <netinet/in.h>
#define socket_address sockaddr_in
#endif

#ifdef _WIN32
// TODO
#endif

struct ReceiveResult {
    bool success;
    bool ignore;
    uint32_t address;
    uint16_t port;
    std::string repr;
    std::string data;
};

struct LocalAddress {
    uint32_t address;
    uint32_t mask;
};

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

    int m_socket {};
    socket_address m_addr {};
};

class ServerSocket {
public:
    explicit ServerSocket(uint16_t port)
        : m_port { port }
    {
    }

    bool bind();

    ReceiveResult receive();

private:
    uint16_t m_port;

    int m_socket {};
    socket_address m_addr {};

    std::vector<LocalAddress> m_ignore {};
};
