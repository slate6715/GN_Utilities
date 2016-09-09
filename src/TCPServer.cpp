#include "TCPServer.h"

using boost::asio::ip::tcp;

namespace util {

	template <class ConnClass>
	TCPServer<ConnClass>::TCPServer(io_s &io_service) : _io(io_service)
	{
		_port = 0;
	}

	/* template <class ConnClass>
	TCPServer<ConnClass>::TCPServer(const TCPServer& orig) {
		_addr = orig._addr;
		_port = orig._port;
		_acceptor = NULL;
	} */ // No copy constructors

	template <class ConnClass>
	TCPServer<ConnClass>::~TCPServer(void)
	{
	}

	template <class ConnClass>
	void TCPServer<ConnClass>::connect() {

		// Create a new acceptor with our IO async service
		_acceptor.reset(new tcp::acceptor(_io, tcp::endpoint(tcp::v4(), _port)));

		start_accept();
	}

	template <class ConnClass>
	void TCPServer<ConnClass>::connect(const char *addr, unsigned int port) {
		change_site(addr, port);

		connect();
	}

	template <class ConnClass>
	void TCPServer<ConnClass>::start_accept()
	{
		ConnClass::cptr listen_socket = ConnClass::create(_io);

		_acceptor->async_accept(listen_socket->socket(),
			boost::bind(&TCPServer::handle_accept, this, listen_socket,
				boost::asio::placeholders::error));
	}


	/*
	* change_site - Changes the site inforation and if connected, disconnects
	*				so there's no conflict between connection and information
	*
	* Params:	addr - the IP (eventually support DNS) site to connect to
	*			port - the IP port
	*
	* Returns:  true if a site was disconnected, false otherwise
	*/

	template <class ConnClass>
	bool TCPServer<ConnClass>::change_site(const char *addr, unsigned int port) {
		bool disconnected = false;

		if (_acceptor != NULL) {
			_acceptor->close();
			disconnected = true;
		} 

		_addr = addr;
		_b_addr.from_string(addr);
		_port = port;
		return disconnected;
	}

	template <class ConnClass>
	void TCPServer<ConnClass>::handle_accept(TCPConnection::cptr new_connection,
		const boost::system::error_code& error) {
		printf("Connect!\n");

		if (!error) {
			new_connection->start();
			start_accept();
		}

	}


} // namespace util
