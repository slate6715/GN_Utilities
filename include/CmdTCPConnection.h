#pragma once
#include "stdafx.h"
#include "TCPConnection.h"
#include <boost/property_tree/ptree.hpp>
#include "Image.h"
#include "CommandHandler.h"
#include <functional>

namespace pt = boost::property_tree;

namespace util {

	template <class CmdClass>
	class CmdTCPConnection :
		public TCPConnection
	{
	public:
		virtual ~CmdTCPConnection();

		virtual void start();

		// create static function - create a new CmdTCPConnection object with the passed in
		//                          io_Service and a shared pointer to the parent type.  Used
		//						    by the server when new connections are received.  If no
		//							CommandHandler is passed in, it creates its own with default
		//							settings
		static cptr create(boost::asio::io_service& io_service)
		{
			return cptr(new CmdTCPConnection<CmdClass>(io_service));
		}

		static cptr create(boost::asio::io_service& io_service, std::shared_ptr<CommandHandler> cmd_h)
		{
			return cptr(new CmdTCPConnection<CmdClass>(io_service, cmd_h));
		}

		// send_image - Takes an image and transmits it to the distant end.  The first variant
		//				takes in a util::Image class and converts it to jpeg for compression, 
		//				then uses the second variant to send the data via a passed in vector buffer
		//				that contains the image.  It lets the distant end know via JSON commands 
		//				that an image is coming and sends the image.  If image parameters are 0, then
		//				the distant end function must interpret from the image header
		// Parameters:  img - the image that we're sending
		//				imgtype - the type string of the image.  Supported:  'jpg'
		//				x - the horizontal size of the image in pixels (0 = 
		//				y - the vertical size of the image in pixels
		//				csize - the bit color size

		void send_image(util::Image &img);
		void send_image(std::shared_ptr<std::vector<unsigned char>> img, const char *imgtype=NULL);

		bool execute_json();

	protected:
		CmdTCPConnection(boost::asio::io_service& io_service);
		CmdTCPConnection(boost::asio::io_service& io_service, std::shared_ptr<CmdClass> cmd_h);

		// Overload of TCPConnection functions to handle specialized activities
		virtual void process_read_data();

		virtual void process_bulk_data();

		std::shared_ptr<CommandHandler> _cmd_h;

		pt::ptree _last_cmd;
	};

} // Namespace util

#include "../src/CmdTCPConnection.cpp"
