#ifndef WINUDPSERVER_H
#define WINUDPSERVER_H

#include "c:\storage\research\code\gnutilities\include\udpserver.h"

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

#endif // WIN32UDPSERVER_H