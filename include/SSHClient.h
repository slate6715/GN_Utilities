#pragma once

#ifdef USE_SSH

#include <string>
#include "libssh\libssh.h"

namespace util {

class SSHClient
{
public:
	SSHClient(void);
	virtual ~SSHClient(void);

	SSHClient(const char *addr, unsigned int port, const char *keyfile=NULL);

	void setConnection(const char *addr, unsigned int port, const char keyfile);
	

protected:
	std::string _addr;
	unsigned int _port;
	std::string _keyfile;

	ssh_session _session;
};

} // namespace util

#endif  // USE_SSH