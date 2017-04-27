#ifdef _WIN32
#include "stdafx.h"
#endif
#include "UDPClient.h"
#include "miscutils.h"

namespace util {

UDPClient::UDPClient(void)
{
	_sock_open = false;
}

UDPClient::UDPClient(const char *addr, unsigned int port) {
	_addr = addr;
	_port = port;
	_sock_open = false;
}

UDPClient::~UDPClient(void)
{
}

void UDPClient::connect(const char *addr, unsigned int port) {
	_addr = addr;
	_port = port;
	connect();
}

void UDPClient::connect() {
	throw util::RuntimeException("connect() parent class function called when child class should have");
}

void UDPClient::close() {
}


} // namespace util
