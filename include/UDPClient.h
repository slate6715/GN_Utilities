#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <string>

namespace util {
class UDPClient
{
public:
	UDPClient(void);
	UDPClient(const char *addr, unsigned int port);

	virtual ~UDPClient(void);
	virtual void connect(const char *addr, unsigned int port);
	virtual void connect();
	virtual void close();

protected:
	std::string _addr;
	unsigned int _port;
	bool _sock_open;
};

} // namespace util

#endif // UDPCLIENT_H