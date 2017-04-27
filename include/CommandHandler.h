#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include "TCPConnection.h"
#include "Image.h"
#include "miscutils.h"

class CmdTCPConnection;

namespace pt = boost::property_tree;

namespace util {

        class UnsupportedCommand : public std::exception
        {
        public:
                UnsupportedCommand(std::string &cmd, std::string &msg) : _cmd(cmd), _msg(msg) {};
                UnsupportedCommand(const char *cmd, const char *msg) : _cmd(cmd), _msg(msg) {};
                ~UnsupportedCommand() throw() {};
                const char *what() { return _msg.c_str(); };
                const char *cmd() { return _cmd.c_str(); };

        private:
                std::string _cmd;
                std::string _msg;
        };

	class CommandHandler
	{
	public:
		CommandHandler();
		virtual ~CommandHandler();

		
		// handle_command - takes a ptree as input and looks for the cmd field
		//					executing the appropriate function call for the
		//					command given.  Some function calls expect overload.
		//
		// Parameters:  cmdtree - the ptree with the command information
		//				conn - the connection to handle commands, like read_image
		//					   that require further access to the connection
		//
		//		Returns - true if the command was fully executed
		//				  false if it requires async activity to complete (like reading an image)
		//
		//		Throws: InvalidCommand exception if a command is read that isn't supported
		//				or that requires a connection and none was passed in

		template <typename Conn>
		bool handle_command(pt::ptree &cmdtree) {
			return handle_command_c(cmdtree);
		}

		template <typename Conn>
		bool handle_command(pt::ptree &cmdtree, Conn *conn) {

			return handle_command_c(cmdtree, conn);
		}

	protected:


	private:
		pt::ptree cur_command;

		// Start the async read operations to read in the image 
		// (or if all the data is here, just read it in)
		template <typename Conn>
		void read_image(pt::ptree &cmdtree, Conn &conn) {
			// Get the expected size so we can ensure we read in that much
			std::string &sizestr = cmdtree.get<std::string>("params.imgsize");
			int imgsize = boost::lexical_cast<int>(sizestr);

			// Start the async reads to get all the image
			conn.start_bulk_read(imgsize);

		}

		template <typename Conn>
		bool handle_command_c(pt::ptree &cmdtree, Conn *conn) {

			std::string cmd = cmdtree.get<std::string>("cmd");

			if (cmd == "sendimg") {
				if (conn == NULL) {
					throw util::UnsupportedCommand("sendimg",
						"CmdTCPConnection command sendimg received, no connection passed in as param");
				}

				read_image(cmdtree, *conn);
				return false; // Requires async activity to complete
			}
			else {
				std::string msg("Unknown CmdTCPConnection command sent via JSON:  ");
				msg += cmd;
				throw util::IOException(msg);
			}
			return true;
		}

		// If the connection pointer doesn't exist, then (what do we do????)
		bool handle_command_c(pt::ptree &cmdtree) {

                        std::string cmd = cmdtree.get<std::string>("cmd");

			// No use for this instance yet
			if (cmd == "sendimg") {
                        	throw util::UnsupportedCommand("sendimg",
                                   "CmdTCPConnection command sendimg received, no connection passed in as param");
			} 
                        else {
                           std::string msg("Unknown CmdTCPConnection command sent via JSON:  ");
                           msg += cmd;
                           throw util::IOException(msg);
                        } 
			return true;
		}
	};


}
