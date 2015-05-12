#ifndef WINUDPSERVER_H
#define WINUDPSERVER_H

#ifdef WIN32

#include "UDPServer.h"

namespace util {

class WinUDPServer :
	public UDPServer
{
public:
	WinUDPServer(void);

    WinUDPServer(const char *addr, unsigned int port);
    WinUDPServer(const WinUDPServer& orig);
    
	virtual ~WinUDPServer(void);
    
    virtual void connect();
    virtual void connect(const char *addr, unsigned int port);

    int accept_connection(unsigned int block_ms);
};

} // namespace util

#endif // ifdef WIN32

#endif // WIN32UDPSERVER_H