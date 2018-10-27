#ifndef GHOST_SERVER_HPP
#define GHOST_SERVER_HPP

#include <memory>

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/ClientHandler.hpp>

namespace ghost
{
	/**
	 * @brief Server that awaits Client connections.
	 * 
	 * For each incoming connection, the provided implementation of the ClientHandler
	 * will be called. See the ClientHandler documentation for more information.
	 * 
	 * A Server can be created via a ConnectionManager, which will also
	 * manage its lifetime.
	 * 
	 * The configuration provided in the constructor is used to set up the connection
	 * parameters such as the target IP address, port number or threading or blocking
	 * parameters.
	 * 
	 * Particular implementations of Connection classes may define new connection
	 * parameters which can be passed through the ConnectionConfiguration.
	 */
	class Server : public Connection
	{
	public:
		/**
		 * @brief Constructor of a Server.
		 * 
		 * @param configuration the Server's configuration.
		 */
		Server(const ConnectionConfiguration& configuration);

		virtual ~Server() = 0;

		/**
		 * @brief Sets the handler for incoming Client connections. The handler
		 * will be called for every connection.
		 * 
		 * @param handler custom implementation of a ClientHandler
		 */
		virtual void setClientHandler(std::shared_ptr<ClientHandler> handler) = 0;
	};

	inline Server::~Server() {}
}

#endif //GHOST_SERVER_HPP
