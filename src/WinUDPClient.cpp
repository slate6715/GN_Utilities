#include "stdafx.h"

#ifdef _WIN32

#include "WS2tcpip.h"
#include "WinUDPClient.h"

namespace util {

WinUDPClient::WinUDPClient(void)
{
	_sock_open = false;
}


WinUDPClient::~WinUDPClient(void)
{
	close();
}

void WinUDPClient::connect() {
	int slen=sizeof(_si_addr);
//    char buf[BUFLEN];
//    char message[BUFLEN]; // later use
    WSADATA wsa;
 
    //Initialise winsock
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
		std::stringstream msg;
		msg << "Connection Failed. Error Code : " << WSAGetLastError();
        throw util::NetworkException(msg.str());
    }
     
    //create socket
    if ( (_sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
		std::stringstream msg;
        msg << "socket creation failed with error code : " << WSAGetLastError();
        throw util::NetworkException(msg.str());
    }
     
    //setup address structure
	char inet_str[INET_ADDRSTRLEN];
	memset(inet_str, 0, INET_ADDRSTRLEN);

    memset((char *) &_si_addr, 0, sizeof(_si_addr));
    _si_addr.sin_family = AF_INET;
    _si_addr.sin_port = htons(_port);
	inet_pton(AF_INET, _addr.c_str(), inet_str);
// 	_si_addr.sin_addr.S_un.S_addr = inet_str;
// Re-add the previous line after I test boost

	return;
}

void WinUDPClient::send(const char *msg) {
	//send the message
    if (sendto(_sock, msg, strlen(msg) , 0 , (struct sockaddr *) &_si_addr, sizeof(_si_addr)) == SOCKET_ERROR)
    {
		std::stringstream msg;
		msg << "sendto() failed with error code : " << WSAGetLastError();
        throw util::NetworkException(msg.str());
    }
    /*     
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
         
        puts(buf);
    }*/

}

void WinUDPClient::close() {
    closesocket(_sock);
    WSACleanup();
}

} // namespace util

#endif // ifdef _WIN32