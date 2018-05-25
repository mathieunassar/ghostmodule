#ifndef GHOST_SERVER_HPP
#define GHOST_SERVER_HPP

#include <memory>

#include "internal/Server.hpp"
#include "Connection.hpp"
#include "Client.hpp"

namespace ghost
{
	class Server : public Connection
	{
	protected:
		/**
		 * Processes a client received by the server.
		 * The user can store the client by copying the Client object in an appropriate place and
		 * setting the "keepClient" flag to true.
		 * @author	Mathieu Nassar
		 * @date	21.05.2018
		 * @param [in,out]	client	  	connected client to process.
		 * @param [in,out]	keepClient	set keepClient to true to prevent the server from closing the
		 * 								connection to the client.
		 * @return	true to let the server continue processing clients. Returning false is equivalent to
		 * 			calling "stop" method.
		 */
		virtual bool onNewClient(Client& client, bool& keepClient) = 0;

		/**
		 * Releases the client described by client.
		 * @author	Mathieu Nassar
		 * @date	21.05.2018
		 * @param	client	The client.
		 */
		void releaseClient(const Client& client);

	private:
		std::shared_ptr<internal::Server> _internal;
	};
}

#endif //GHOST_SERVER_HPP
