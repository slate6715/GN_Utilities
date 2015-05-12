#ifndef LINUXUDPSERVER_H
#define LINUXUDPSERVER_H

#ifndef _WIN32

#include "udpserver.h"

namespace util{

class LinuxUDPServer :
	public UDPServer
{
public:
	LinuxUDPServer(void);
	virtual ~LinuxUDPServer(void);

    LinuxUDPServer();
	LinuxUDPServer(const char *addr, unsigned int port) : UDPServer(addr, port) {};
    LinuxUDPServer(const UDPServer& orig);
    
    virtual ~LinuxUDPServer();
    
    void connect();
    void connect(const char *addr, unsigned int port);

    int accept_connection(unsigned int block_ms);

};

} // namespace util

#endif // _WIN32

#endif // if LINUXUDPSERVER_H

