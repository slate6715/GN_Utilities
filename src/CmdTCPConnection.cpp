#include "CmdTCPConnection.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include "miscutils.h"

namespace util {

	template <class CmdClass>
	CmdTCPConnection<CmdClass>::CmdTCPConnection(boost::asio::io_service& io_service) : 
													TCPConnection(io_service)
	{
		_cmd_h = std::make_shared<CmdClass>();
	}

	template <class CmdClass>
	CmdTCPConnection<CmdClass>::CmdTCPConnection(boost::asio::io_service& io_service, std::shared_ptr<CmdClass> cmd_h) :
		TCPConnection(io_service), _cmd_h(cmd_h)
	{

	}

	template <class CmdClass>
	CmdTCPConnection<CmdClass>::~CmdTCPConnection()
	{

	}

	template <class CmdClass>
	void CmdTCPConnection<CmdClass>::start() {
		TCPConnection::start();
	}

	template <class CmdClass>
	void CmdTCPConnection<CmdClass>::process_read_data() {

		// Loop through the read buffers and handle their JSON until empty.  Relies on the
		// execute_json function to pop the read buffer after it's no longer useful.  
		// execute_json also needs to call do_read or do_bulk_read as appropriate to
		// keep reading from the sockets
		while (_read_list.size() > 0) {
			if (!execute_json())
				break;  // Async ops required to complete the last json, break out
		}
	}

	template <class CmdClass>
	void CmdTCPConnection<CmdClass>::process_bulk_data() {
		TCPConnection::process_bulk_data(); // Default behavior, mostly ignores the results.  
	}


	template <class CmdClass>
	bool CmdTCPConnection<CmdClass>::execute_json() {
		// User should have checked for a non-empty read buffer first
		if (_read_list.size() <= 0) {
			throw util::RuntimeException("execute_json called on empty read buffer.");
		}

		// We're going to assemble the read buffers into one buffer to make sure
		// we have the whole JSON command in one string
		
		std::unique_ptr<std::string> read_buf;
		if (_read_list.size() > 1) {
			// Get the size of the buffer we need first
			int bufsize = 0;
			for (unsigned int i = 0; i < _read_list.size(); i++) {
				bufsize += _read_list[i]->size();
			}

			// Now build the new buffer
			read_buf.reset(new std::string());
			while (_read_list.size() > 0) {
				read_buf->append(*_read_list.front()->begin(), *_read_list.front()->end());
				_read_list.pop_front();
			}
		}
		else {
			// Else only one, just get it
			read_buf = get_next_buf_str();
		}

		// Do a quick verification to make sure we got the whole JSON and
		// mark the end of the JSON to prepare to read in the next buffer
		
		// if the first character is not a left bracket, then we have extra
		// data at the beginning that shouldn't be there.  Raise an error
		// and eliminate it
		if ((*read_buf)[0] != '{') {
			// Find where the bracket starts
			unsigned int i = 0;
			while ((i < read_buf->size()) && ((*read_buf)[i] != '{'))
				i++;

			boost::system::error_code error = boost::system::errc::make_error_code(boost::system::errc::bad_message);
			std::string msg("Non-JSON string found at beginning of buffer, skipping.  Buffer was: ");
			msg += read_buf->substr(0, i);
			handle_error(error, "execute_json", msg.c_str());
			// If there is no JSON text here, then we're done..nuke it all and
			// prepare to read more from the socket
			if (i == read_buf->size()) {
				TCPConnection::do_read();
				return false;
			}

			// Else eliminate the extra stuff at the beginning and move on
			read_buf->erase(read_buf->begin(), read_buf->begin() + i);
		}

		// Now find the last bracket
		unsigned int last_pos = 1;
		unsigned int bracket_count = 1;
		while ((last_pos < read_buf->size()) && (bracket_count > 0)) {
			if ((*read_buf)[last_pos] == '{')
				bracket_count++;
			else if ((*read_buf)[last_pos] == '}')
				bracket_count--;
			last_pos++;

			// If we don't have a full JSON, then we should exit and wait for
			// the rest to arrive
			if ((last_pos == read_buf->size()) && (bracket_count > 0)) {
				TCPConnection::do_read();
				addto_readlist(*read_buf, read_buf->size());
				return false;
			}
		}

		// Put the rest back into the read_buffer
		if (last_pos + 1 < read_buf->size()) {
			addto_readlist(*read_buf, last_pos, read_buf->size() - last_pos);
			read_buf->erase(read_buf->begin() + last_pos + 1, read_buf->end());
		}

		// Read in the JSON tree and fail if there's a problem
		std::cout << *read_buf << "\n";
		std::istringstream json_buf(*read_buf);  // Because it thinks a string is a file but an istream is not
		pt::read_json(json_buf, _last_cmd);

		// Determine the command that was sent and handle appropriately
		return _cmd_h->handle_command<CmdTCPConnection<CmdClass>>(_last_cmd, this);
	}

#ifdef USE_OPENCV
	template <class CmdClass>
	void CmdTCPConnection<CmdClass>::send_image(util::Image &img) {
		std::shared_ptr<std::vector<unsigned char>> imgbuf = std::make_shared<std::vector<unsigned char>>();

		cv::imencode(".jpg", img.getImage(), *imgbuf);

		
		return send_image(imgbuf, "jpg");
	}

	template <class CmdClass>
	void CmdTCPConnection<CmdClass>::send_image(std::shared_ptr<std::vector<unsigned char>> img, const char *imgtype) {
		pt::ptree cmdtree;

		// Put together the command to provide information about the image about to arrive
		cmdtree.put("cmd", "sendimg");
		cmdtree.put("params.imgtype", (imgtype == NULL) ? "unk" : "jpg");
		cmdtree.put("params.imgsize", img->size());

		std::stringstream buf;
		pt::write_json(buf, cmdtree);
		write_text(buf.str().c_str());

		// Now write the image data
		add_writebuf(img);
 		do_write();

	}
#endif

} // Namespace util
