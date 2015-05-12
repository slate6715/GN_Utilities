#pragma once

#ifdef USE_SSH

namespace util {

class SSHServer
{
public:
	SSHServer(void);
	virtual ~SSHServer(void);
};

} // namespace util

#endif // USE_SSH