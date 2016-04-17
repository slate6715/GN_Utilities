#pragma once

#include <boost/asio.hpp>

namespace util{

class UDPServer
{
public:
    UDPServer(void);

    UDPServer(const char *addr, unsigned int port) {};
    UDPServer(const UDPServer& orig);
    
    virtual ~UDPServer();
    
    void connect();
    void connect(const char *addr, unsigned int port);

    int accept_connection(unsigned int block_ms);

protected:
    unsigned int _port;
    std::string _addr;
};

} // namespace util



