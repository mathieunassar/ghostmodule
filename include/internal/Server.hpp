#ifndef GHOST_INTERNAL_SERVER_HPP
#define GHOST_INTERNAL_SERVER_HPP

#include "Connection.hpp"
#include "Client.hpp"

namespace ghost
{
	namespace internal
	{
		class Server : public Connection
		{
		protected:
			/**
			* Releases the client described by client.
			* @author	Mathieu Nassar
			* @date	21.05.2018
			* @param	client	The client.
			*/
			void releaseClient(const Client& client);
		};
	}
	
}

#endif //GHOST_INTERNAL_SERVER_HPP
