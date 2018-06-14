#ifndef GHOST_INTERNAL_CLIENT_HPP
#define GHOST_INTERNAL_CLIENT_HPP

#include "Connection.hpp"
#include "../Message.hpp"

namespace ghost
{
	namespace internal
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
			* Encodes and sends the message given as argument.
			* @author	Mathieu Nassar
			* @date	21.05.2018
			* @param	message	message to send.
			* @return	A long.
			*/
			virtual long send(const ghost::Message& message) = 0;
		};
	}
	
}

#endif //GHOST_INTERNAL_CLIENT_HPP
