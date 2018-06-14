#ifndef GHOST_INTERNAL_SERVER_HPP
#define GHOST_INTERNAL_SERVER_HPP

#include <functional>

#include "Connection.hpp"
#include "Client.hpp"

namespace ghost
{
	namespace internal
	{
		class Server : public Connection
		{
		public:
			/**
			* Releases the client described by client.
			* @author	Mathieu Nassar
			* @date	21.05.2018
			* @param	client	The client.
			*/
			//virtual void releaseClient(const Client& client) = 0;

			virtual void setNewClientCallback(std::function<bool(Client&)> callback) = 0;
		};
	}
	
}

#endif //GHOST_INTERNAL_SERVER_HPP
