#ifndef GHOST_CLIENTHANDLER_HPP
#define GHOST_CLIENTHANDLER_HPP

#include <ghost/connection/Client.hpp>

namespace ghost
{
	/**
	 * @brief Handler for a new client connection.
	 * 
	 * Implementations of this base class will be called for each new client connection.
	 * 
	 * The handle method will specifically be called with a Client object passed,
	 * which represents the remote client connection.
	 * 
	 * A client handler may choose to keep the connection to the client alive by modifying
	 * the value of "keepClientAlive" to true.
	 * 
	 * A client handler may also shut down the Server by returning false on the call to
	 * "handle".
	 */
	class ClientHandler
	{
	public:
		virtual ~ClientHandler() = 0;

		/**
		 * @brief Handles a client connection.
		 * 
		 * @param client represents the client connection. A Reader or a Writer may be
		 * created from the client to communicate.
		 * @param keepClientAlive set it to true to prevent the server from closing this
		 * connection. Note that if the server is shut down, all connections will be closed
		 * @return true if the server should continue to run
		 * @return false to shutdown the server
		 */
		virtual bool handle(std::shared_ptr<Client> client, bool& keepClientAlive) = 0;
	};

	inline ClientHandler::~ClientHandler() {}
}

#endif //GHOST_CLIENTHANDLER_HPP
