
#ifdef _WIN32
#include "stdafx.h"
#endif

#ifndef _WIN32

#include "LinuxUDPServer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include "miscutils.h"
#include "strings.h"

namespace util {

LinuxUDPServer::LinuxUDPServer(void)
{
}

LinuxUDPServer::LinuxUDPServer(const LinuxUDPServer& orig) {
    _addr = orig._addr;
    _port = orig._port;
    
}

LinuxUDPServer::~LinuxUDPServer(void)
{
    close(_sockfd);
}


void LinuxUDPServer::connect() {
    struct sockaddr_in serv_addr, cli_addr;

    if (_port >= 65536) {
        std::stringstream msg;
        msg << "Invalid Port: " << _port << ".  Valid is 1-65535";
        throw util::NetworkException(msg.str().c_str());
    }
    
    _sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (_sockfd < 0) {
        throw util::NetworkException("Error opening socket");
    }
    
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(_port);
    if (bind(_sockfd, (struct sockaddr *) &serv_addr, 
             sizeof(serv_addr)) < 0) {
        std::stringstream msg;
        msg << "Failed binding server to " << _addr << " " << _port;
        throw util::NetworkException(msg.str().c_str());
    }
    
}

void LinuxUDPServer::connect(const char *addr, unsigned int port) {
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

int LinuxUDPServer::accept_connection(unsigned int block_ms) {
    
    pollfd p[POLLSET_SIZE];
    int numfds = POLLSET_SIZE;
    
    p[0].fd = _sockfd;
    p[0].events = POLLIN;
    p[0].revents = 0;
    
    poll(p, numfds, block_ms);
    if (numfds == 0) {
        return 0;
    }
    
    for (int i=0; i<numfds; i++) {
        
    }
    
}


} // namespace util

#endif // _WIN32
