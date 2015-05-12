#ifndef LINUXUDPSERVER_H
#define LINUXUDPSERVER_H

#ifndef _WIN32

#include "UDPServer.h"

namespace util{

class LinuxUDPServer :
	public UDPServer
{
public:
    LinuxUDPServer(void);

    LinuxUDPServer(const char *addr, unsigned int port);
    LinuxUDPServer(const LinuxUDPServer& orig);
    
    virtual ~LinuxUDPServer();
    
    void connect();
    void connect(const char *addr, unsigned int port);

    int accept_connection(unsigned int block_ms);
    
private:
    unsigned int _sockfd;
};

} // namespace util

#endif // _WIN32

#endif // if LINUXUDPSERVER_H

