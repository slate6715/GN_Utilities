#ifdef _WIN32
#include "stdafx.h"
#endif
#include "UDPServer.h"

namespace util {

UDPServer::UDPServer(void)
{
}

UDPServer::UDPServer(const UDPServer& orig) {
    _addr = orig._addr;
    _port = orig._port;
    
}

UDPServer::~UDPServer(void)
{
}

void UDPServer::connect() {
}

void UDPServer::connect(const char *addr, unsigned int port) {
}

/*
 * accept_connection - looks for a connection for the amount of time given
 *      in block_ms and, if one is found, accepts it
 * 
 * Params:  block_ms - number of milliseconds to block during the poll
 * 
 * Returns:  0 for no connection found, 1 for a connection
 */

int UDPServer::accept_connection(unsigned int block_ms) {
   return -1; 
}


} // namespace util
