#ifndef UDPSERVER_H
#define UDPSERVER_H

#ifndef _WIN32

namespace util{

class UDPServer
{
public:
	UDPServer(void);
	virtual ~UDPServer(void);

    UDPServer();
    UDPServer(const char *addr, unsigned int port) : UDPServer(addr, port) {};
    UDPServer(const UDPServer& orig);
    
    virtual ~UDPServer();
    
    void connect();
    void connect(const char *addr, unsigned int port);

    int accept_connection(unsigned int block_ms);

};

} // namespace util

#endif // _WIN32

#endif // if UDPSERVER_H

