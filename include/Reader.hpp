#ifndef GHOST_READER_HPP
#define GHOST_READER_HPP

#include "Message.hpp"
#include "MessageHandler.hpp"

namespace ghost
{
	template<typename MessageType>
	class Reader
	{
	public:
		/**
		* Receives from the connection and decodes the message.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		* @param [in,out]	message	variable containing the output message.
		* @return	True if it succeeds, false if it fails.
		*/
		virtual bool read(MessageType& message) = 0;

		/**
		* Decodes the last message received.
		* @author	Mathieu Nassar
		* @date	20.06.2018
		* @param [in,out]	message	variable containing the output message.
		* @return	True if it succeeds, false if it fails.
		*/
		virtual bool lastRead(MessageType& message) = 0;

		virtual std::shared_ptr<ghost::MessageHandler> addMessageHandler() = 0;
	};
}

#endif //GHOST_WRITER_HPP
