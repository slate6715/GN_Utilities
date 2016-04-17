#include "stdafx.h"

#ifdef WIN32

#include "WinUDPServer.h"

namespace util {

WinUDPServer::WinUDPServer(void)
{
}


WinUDPServer::WinUDPServer(const WinUDPServer& orig) {
    _addr = orig._addr;
    _port = orig._port;
    
}

WinUDPServer::~WinUDPServer(void)
{
}

void WinUDPServer::connect() {
    
}

void WinUDPServer::connect(const char *addr, unsigned int port) {
    _addr = addr;
    _port = port;
    connect();    
}

/*
 * accept_connection - looks for a connection for the amount of time given
 *      in block_ms and, if one is found, accepts it
 * 
 * Params:  block_ms - number of milliseconds to block during the poll
 * 
 * Returns:  0 for no connection found, 1 for a connection
 */

#define POLLSET_SIZE 32

int WinUDPServer::accept_connection(unsigned int block_ms) {
    return -1;
}

} // namespace util

#endif //ifdef WIN32