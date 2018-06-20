#ifndef GHOST_SERVER_HPP
#define GHOST_SERVER_HPP

#include <memory>

#include "internal/Server.hpp"
#include "Connection.hpp"
#include "ClientHandler.hpp"

namespace ghost
{
	class Server : public Connection
	{
	public:
		/**
		 * Sets the handler which will process the clients of this server.
		 * @author	Mathieu Nassar
		 * @date	15.06.2018
		 * @param	handler	The handler.
		 */
		virtual void setClientHandler(std::shared_ptr<ClientHandler> handler) = 0;
	};
}

#endif //GHOST_SERVER_HPP
