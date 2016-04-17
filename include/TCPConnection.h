#pragma once

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <deque>
#include <vector>

using boost::asio::ip::tcp;

namespace util {

	class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
	{
	public:

		virtual ~TCPConnection() {};

		void start_connect(const char *addr, unsigned int port);

		// write_text - Writes text to the output socket asynchronously and returns immediately		
		// Parameter: msg - string buffer or pointer to a buffer with the message terminated by a \0

		void write_text(std::string &msg);
		void write_text(const char *msg);

		// read_text - Pulls text from the next buffer in the queue, assuming there is text to pull,
		//             and dumps it into the passed in parameter		
		// Parameter: buf - string vector to place the results into
		// Return:  -1 if no reads available, number of reads left if successful

		int read_text(std::string &buf);

		// handle_error - function to generally be overloaded and handle network errors
		void handle_error(const boost::system::error_code& error, const char *functname = NULL, const char *message = NULL);

		typedef boost::shared_ptr<TCPConnection> cptr;

		// create static function - create a new CmdTCPConnection object with the passed in
		//                          io_Service and a shared pointer to the parent type.  Used
		//						    by the server when new connections are received.  Must be
		//							overloaded for all child classes of TCPConnection the server uses
		static cptr create(boost::asio::io_service& io_service)
		{
			return cptr(new TCPConnection(io_service));
		}

		tcp::socket& socket() { return _socket; }

		
		// start - Called when a new TCP connection is initiated.  This function is called
		//		by the async server handler and should normally be overloaded in that instance
		//		to handle application-specific actions

		virtual void start();

		// Sets up the async handler to read in the number of bytes specified
		// in the parameter
		void start_bulk_read(unsigned long num_bytes);

		// Gets the next read-in buffer.  Pops the buffer off the stack
		int get_total_readlistsize();
		std::unique_ptr<std::string> get_next_buf_str();
		std::unique_ptr<std::vector<unsigned char>> get_next_buf_uchar();

		static const int max_readlen;

		std::unique_ptr<std::vector<unsigned char>> pull_bulk_data();

	protected:
		// Constructor called by static factory function above
		TCPConnection(boost::asio::io_service& io_service);

		// **********************************************************************
		// **** Functions to be overloaded to handle events in specific ways ****
		// **********************************************************************

		// process_read_data - Called after the data has been read into the _read_list
		//                     vector stack and is waiting for processing.  This
		//					   function should call do_read to set up the async
		//					   handler to continue reading from the socket or do other
		//					   user-defined actions
		virtual void process_read_data();

		// process_read_data - Called after the specified amount of bulk data has
		//					   been read in and stored in the _bulk_data vector.
		//					   It is necessary to call do_read or do_bulk_read after
		//					   to continue reading data from the socket
		virtual void process_bulk_data();

		// write_complete - virtual function with no default behavior called when
		//                  the write buffer has finally emptied itself
		virtual void write_complete() {};

		virtual void continue_connect(const boost::system::error_code& error,
			boost::asio::ip::tcp::resolver::iterator iterator);
		virtual void handle_connect(const boost::system::error_code& error,
			boost::asio::ip::tcp::resolver::iterator iterator);

		// Sets up the async handler to read in the next group of bytes
		void do_read();

		// Adds a vector of data to the write buffer
		int add_writebuf(std::shared_ptr<std::vector<unsigned char>> &buf);

		// Starts the asynchronous write buffer handler to write all the data
		void do_write();

		void addto_readlist(std::string &buf, unsigned int bytes, 
														unsigned int frompos=0);
		void addto_readlist(std::vector<unsigned char> &buf, unsigned int bytes, 
														unsigned int frompos=0);

		tcp::resolver _resolver;

		std::deque<std::shared_ptr<std::vector<unsigned char>>> _read_list;

		std::vector<unsigned char> _read_buf;

	private:

		// Used to continue the bulk read until the right amount is read in.  Not to be overloaded
		void do_bulk_read();

		// Event handlers for the boost asio io process
		virtual void handle_write(const boost::system::error_code& error,
			size_t bytes_transferred);
		virtual void handle_read(const boost::system::error_code& error,
			size_t bytes_transferred);
		virtual void handle_bulk_read(const boost::system::error_code& error,
			size_t bytes_transferred);


		std::string _message;
		std::deque<std::shared_ptr<std::vector<unsigned char>>> _write_bufs;

		// Error messages for asynchronous operations to get why the connection may
		// have failed
		boost::system::error_code _last_err;

		// Due to shared pointer issues, all async calls must be handled by the TCPConnection
		// class.  
		boost::asio::ip::tcp::socket _socket;

		// Used for reading in raw data gradually
		unsigned long _expected_bulk_size;
		std::vector<unsigned char> _bulk_buf;


	};

}

