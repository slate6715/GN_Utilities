/* 
 * File:   UDPServer.cpp
 * Author: George Noel
 * 
 * Creates and manages a simple UDP server with methods for transmitting common
 * data types
 * 
 * Created on 14 March 2015, 07:20
 */

#include "UDPServer.h"

namespace util {

UDPServer::UDPServer() {
    _port = 0;
    _sockfd = 0;
};

UDPServer::UDPServer(const char *addr, unsigned int port) {
    _addr = addr;
    _port = port;
    _sockfd = 0;
}



} // namespace util