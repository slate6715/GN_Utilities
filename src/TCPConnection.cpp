#ifdef _WIN32
#include "stdafx.h"
#endif
#include "TCPConnection.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "miscutils.h"

using boost::asio::ip::tcp;

namespace util {


	const int TCPConnection::max_readlen = 5000;

	TCPConnection::TCPConnection(boost::asio::io_service& io_service)
									: _socket(io_service), _resolver(io_service), _read_buf(max_readlen)
	{
		_expected_bulk_size = 0;
	}


	void TCPConnection::start_connect(const char *addr, unsigned int port) {
		std::string port_s = boost::lexical_cast<std::string>(port);
		_resolver.async_resolve(tcp::resolver::query(addr, port_s.c_str()), 
			boost::bind(&TCPConnection::continue_connect, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator));

	}

	void TCPConnection::continue_connect(const boost::system::error_code& error,
		boost::asio::ip::tcp::resolver::iterator iterator) {
		if (!error)
		{
			// Attempt a connection to the first endpoint in the list. Each endpoint
			// will be tried until we successfully establish a connection.
			tcp::endpoint endpoint = *iterator;
			_socket.async_connect(endpoint,
				boost::bind(&TCPConnection::handle_connect, this,
					boost::asio::placeholders::error, ++iterator));
		}
		else
		{
			handle_error(error, "TCPConnection::continue_connect");
		}

	}

	void TCPConnection::handle_connect(const boost::system::error_code& error,
		boost::asio::ip::tcp::resolver::iterator iterator) {
		if (!error)
		{
			start();
		}
		else if (iterator != tcp::resolver::iterator())
		{
			// The connection failed. Try the next endpoint in the list.
			_socket.close();
			tcp::endpoint endpoint = *iterator;
			_socket.async_connect(endpoint,
				boost::bind(&TCPConnection::handle_connect, this,
					boost::asio::placeholders::error, ++iterator));
		}
		else
		{
			handle_error(error, "TCPConnection::handle_connect");
		}
	}

	void TCPConnection::start() {
		do_read();
	}

	void TCPConnection::write_text(std::string &msg) {
		std::shared_ptr<std::vector<unsigned char>> buf = std::make_shared<std::vector<unsigned char>>();
		buf->assign(msg.begin(), msg.end());

		add_writebuf(buf);
	}

	void TCPConnection::write_text(const char *msg) {
		std::string strbuf = msg;
		return write_text(strbuf);
	}

	int TCPConnection::read_text(std::string &buf) {
		//		assert(_read_bufs.size() > 0);
		if (_read_list.size() == 0)
			return -1;

		std::vector<unsigned char> &top_buf = *(_read_list.front());
		buf.assign(top_buf.begin(), top_buf.end());
		_read_list.pop_front();
		return _read_list.size();
	}

	int TCPConnection::add_writebuf(std::shared_ptr<std::vector<unsigned char>> &buf) {
		_write_bufs.push_back(buf);
		return _write_bufs.size();
	}

	void TCPConnection::do_write() {

		assert(_write_bufs.size() > 0);

		// std::cout << "Writing: " << _write_bufs.front()->size() << "\n";
/*
		if (_write_bufs[0]->size() < 6000) {
			std::cout << std::string(_write_bufs[0]->begin(), _write_bufs[0]->end()) << "\n";
		}
		else {
			int startpos = 9975;
			for (unsigned int i = 0; i < 50; i++) {
				std::cout << (int) (*_write_bufs[0])[i + startpos] << " ";
			}
			std::cout << "\n";
		}
		*/

		boost::asio::async_write(_socket, boost::asio::buffer(*_write_bufs.front()),
			boost::bind(&TCPConnection::handle_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

	}

	void TCPConnection::do_read() {

		_socket.async_read_some(boost::asio::buffer(_read_buf, max_readlen),
			boost::bind(&TCPConnection::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void TCPConnection::start_bulk_read(unsigned long num_bytes)
	{
		// Prep our buffers to read in the data
		_expected_bulk_size = num_bytes;
		_bulk_buf.clear();
		_bulk_buf.reserve(num_bytes);

		// First, check the read_list buffer and grab any data in there.
		// If all the data is there, then just call the process_bulk_data
		// function, otherwise set up the async reader
		unsigned long bulk_buf_size = 0;
		while ((_read_list.size() > 0) && ((bulk_buf_size = _bulk_buf.size()) != num_bytes)) {
			assert(bulk_buf_size < num_bytes);

			unsigned long diff = num_bytes - bulk_buf_size;
			if (diff < _read_list.front()->size()) {
				std::vector<unsigned char> &front = *(_read_list.front());
				_bulk_buf.insert(_bulk_buf.end(), front.begin(), front.begin() + diff);
				front.erase(front.begin(), front.begin() + diff);
			}
			else {
				_bulk_buf.insert(_bulk_buf.end(), _read_list.front()->begin(), _read_list.front()->end());
				_read_list.pop_front();
			}
		}

		do_bulk_read();
	}
	
	void TCPConnection::do_bulk_read() {
		assert(_expected_bulk_size > 0);

		_socket.async_read_some(boost::asio::buffer(_read_buf, max_readlen),
			boost::bind(&TCPConnection::handle_bulk_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void TCPConnection::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
		if (!error)
		{
			// If something got received, then add a new vector to the queue to wait for more,
			// otherwise just repoint to the bottom in the queue
			if (bytes_transferred > 0) {
				addto_readlist(_read_buf, bytes_transferred);
			}

			process_read_data();
		}
		else
		{
			handle_error(error, "TCPConnection::handle_read");
		}

	}

	void TCPConnection::process_read_data() {
		// Designed to be overloaded.  Default behavior continues reading
		do_read();
	}


	void TCPConnection::handle_bulk_read(const boost::system::error_code& error,
		size_t bytes_transferred) {
		if (!error)
		{
			// If something got received, then read it into the bulk buffer.  Check the size
			// to make sure we're not reading in too much.  The rest gets handled by the next
			// read.
			if (bytes_transferred > 0) {
				unsigned int remaining = _expected_bulk_size - _bulk_buf.size();
				if (remaining < bytes_transferred) {
					_bulk_buf.insert(_bulk_buf.end(), _read_buf.begin(), _read_buf.begin() + remaining);
					_read_buf.erase(_read_buf.begin(), _read_buf.begin() + remaining);
				    std::vector<unsigned char> rlist(_read_buf.begin() + remaining,
                                _read_buf.begin() + bytes_transferred);
					addto_readlist(rlist, bytes_transferred - remaining); 
				}
				else {
					_bulk_buf.insert(_bulk_buf.end(), _read_buf.begin(), _read_buf.begin() + bytes_transferred);
				}

/*				unsigned int startpos = 9975;
				if (_bulk_buf.size() > startpos + 50) {
				std::cout << "[";
				for (unsigned int i = 0; i < 50; i++) {
					std::cout << (int)_bulk_buf[i + startpos] << " ";
				}
				std::cout << "]\n";
				}
*/
				// Check to see if we got it all and, if so, call the handler
				assert(_bulk_buf.size() <= _expected_bulk_size);
				if (_bulk_buf.size() == _expected_bulk_size) {
					process_bulk_data();
				}
				else {
					do_bulk_read();
				}

			}
			else
			{
				handle_error(error, "TCPConnection::handle_bulk_read");
			}
		}
	}

	void TCPConnection::process_bulk_data() {
		// Designed to be overloaded.  Default behavior continues reading
		do_read();
	}

	std::unique_ptr<std::vector<unsigned char>> TCPConnection::pull_bulk_data() {
		if (_bulk_buf.size() == 0) {
			throw util::RuntimeException("pull_bulk_data:  attempted to pull empty bulk data container.");
		}

		std::unique_ptr<std::vector<unsigned char>> retval(new std::vector<unsigned char>());
		retval->swap(_bulk_buf);
		_expected_bulk_size = 0;
		return retval;
	}


	void TCPConnection::addto_readlist(std::string &buf, unsigned int bytes, unsigned int frompos) {
		assert((frompos + bytes) <= buf.size());

		std::shared_ptr<std::vector<unsigned char>> new_buf =
			std::make_shared<std::vector<unsigned char>>(bytes);

		new_buf->assign(buf.begin() + frompos, buf.begin() + frompos + bytes);
		_read_list.push_back(new_buf);
	}

	void TCPConnection::addto_readlist(std::vector<unsigned char> &buf, unsigned int bytes, 
																		unsigned int frompos) {
		assert((frompos + bytes) <= buf.size());

		std::shared_ptr<std::vector<unsigned char>> new_buf =
			std::make_shared<std::vector<unsigned char>>(bytes);

		new_buf->assign(buf.begin() + frompos, buf.begin() + frompos + bytes);
		_read_list.push_back(new_buf);
	}

	int TCPConnection::get_total_readlistsize() {
		int retval = 0;
		
		auto rl_it = _read_list.begin();
		for (; rl_it != _read_list.end(); rl_it++) { 
			retval += (*rl_it)->size();
		}
		return retval;
	}

	void TCPConnection::handle_write(const boost::system::error_code& error,
		size_t bytes_transferred) {

		if (error) {
			handle_error(error, "TCPConnection::handle_write");
			return;
		}

		assert(_write_bufs.size() > 0);

		if (bytes_transferred > _write_bufs.front()->size()) {
			throw util::RuntimeException("Bytes transferred greater than the buffer size");
		}

		assert(_write_bufs.front()->size() == bytes_transferred);

		_write_bufs.pop_front();

		// If the write buffer isn't empty, continue writing.  Else, call the complete handler
		if (_write_bufs.size() > 0) {
			do_write();
		}
		else {
			write_complete();
		}
	}

	void TCPConnection::handle_error(const boost::system::error_code& error, const char *functname, const char *message) {
		if (functname != NULL)
			std::cout << "Network Error (" << functname << "): ";
		else
			std::cout << "Network Error:  ";

		if (message != NULL) {
			std::cout << message << "\n";
		}
		else {
			std::cout << error.message() << "\n";
		}
	}

	std::unique_ptr<std::string> TCPConnection::get_next_buf_str() {
		if (_read_list.size() <= 0) {
			throw util::RuntimeException("get_next_buf_str called on empty read stack.");
		}

		std::unique_ptr<std::string> retval(new std::string);

		std::vector<unsigned char> &top_buf = *(_read_list.front());
		retval->assign(top_buf.begin(), top_buf.end());
		_read_list.pop_front();
		return retval;
	}

	std::unique_ptr<std::vector<unsigned char>> TCPConnection::get_next_buf_uchar() {
		if (_read_list.size() <= 0) {
			throw util::RuntimeException("get_next_buf_uchar called on empty read stack.");
		}

		std::unique_ptr<std::vector<unsigned char>> retval(new std::vector<unsigned char>());

		std::vector<unsigned char> &top_buf = *(_read_list.front());
		retval->swap(top_buf);
		_read_list.pop_front();
		return retval;
	}

}
