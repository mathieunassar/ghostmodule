#ifndef GHOST_CLIENT_HPP
#define GHOST_CLIENT_HPP

#include <memory>

#include "Connection.hpp"
#include "Message.hpp"
#include "MessageHandler.hpp"

namespace ghost
{
	class Client : public Connection
	{
	public:
		/**
		* Receives from the connection and decodes the message.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		* @param [in,out]	message	variable containing the output message.
		* @return	True if it succeeds, false if it fails.
		*/
		virtual bool receive(ghost::Message& message) = 0;

		/**
		* Decodes the last message received.
		* @author	Mathieu Nassar
		* @date	20.06.2018
		* @param [in,out]	message	variable containing the output message.
		* @return	True if it succeeds, false if it fails.
		*/
		virtual bool lastReceived(ghost::Message& message) = 0;

		/**
		* Encodes and sends the message given as argument.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		* @param	message	message to send.
		* @return	A long.
		*/
		virtual bool send(const ghost::Message& message) = 0;

		virtual std::shared_ptr<ghost::MessageHandler> addMessageHandler() = 0;
	};
}

#endif //GHOST_CLIENT_HPP
