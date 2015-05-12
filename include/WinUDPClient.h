#ifndef WINUDPCLIENT_H
#define WINUDPCLIENT_H

#include "udpclient.h"
#include <WinSock2.h>

#define BUFLEN 2000

namespace util {

class WinUDPClient :
	public UDPClient
{
public:
	WinUDPClient(void);
	virtual ~WinUDPClient(void);

	// Parent function connect(const char *addr, unsigned int port); is available
	virtual void connect();
	virtual void send(const char *msg);
	virtual void close();

private:
    struct sockaddr_in _si_addr;
	int _sock;
};

} // namespace util

#endif // WINUDPCLIENT_H