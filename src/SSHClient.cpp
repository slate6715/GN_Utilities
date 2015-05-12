#ifdef USE_SSH

#include "..\include\SSHClient.h"


namespace util {

SSHClient::SSHClient(void)
{
	_session = ssh_new();
}


SSHClient::~SSHClient(void)
{
	ssh_free(_session);
}

SSHClient::SSHClient(const char *addr, unsigned int port, const char *keyfile) {
	SSHClient();
}

void SSHClient::setConnection(const char *addr, unsigned int port, const char keyfile) {
	_addr = addr;
	_port = port;
	_keyfile = keyfile;


}


} // namespace util

#endif // USE_SSH