#pragma once

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include "TCPConnection.h"

using boost::asio::ip::tcp;

namespace util {

	template <class ConnClass>
	class TCPServer
	{
	public:
		typedef boost::asio::io_service io_s; // Make the code cleaner

		TCPServer(io_s &io);

		TCPServer(io_s &io, const char *addr, unsigned int port) {};
		// TCPServer(io_s &io, const TCPServer& orig);	// No copy constructor allowed

		virtual ~TCPServer();

		void connect();
		void connect(const char *addr, unsigned int port);

		bool change_site(const char *addr, unsigned int port);

	protected:

		virtual void handle_accept(TCPConnection::cptr new_connection,
							const boost::system::error_code& error);

		std::unique_ptr<boost::asio::ip::tcp::acceptor> _acceptor;

		unsigned int _port;
		std::string _addr;

	private:
		void start_accept();

		io_s &_io;
		boost::asio::ip::address _b_addr;
	};

} // namespace util



