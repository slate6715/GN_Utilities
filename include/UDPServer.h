/* 
 * File:   UDPServer.h
 * Author: George Noel
 * 
 * Creates and manages a simple UDP server with methods for transmitting common
 * data types
 *
 * Created on 14 March 2015, 07:20
 */

#ifndef UDPSERVER_H
#define	UDPSERVER_H

#include <string>

namespace util {
    
    
class UDPServer {
public:
    UDPServer();
    UDPServer(const char *addr, unsigned int port);
    UDPServer(const UDPServer& orig);
    
	virtual ~UDPServer() {};
    
	virtual void connect() {};
	virtual void connect(const char *addr, unsigned int port) {};

	virtual int accept_connection(unsigned int block_ms) { return 0; };
    
protected:
    std::string _addr;
    unsigned int _port;
    int _sockfd;
};

} // namespace util

#endif	/* UDPSERVER_H */

