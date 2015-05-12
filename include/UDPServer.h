#ifndef UDPSERVER_H
#define UDPSERVER_H

#ifndef _WIN32

#include <string>

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

#endif // _WIN32

#endif // if UDPSERVER_H

