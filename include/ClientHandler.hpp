#ifndef GHOST_CLIENTHANDLER_HPP
#define GHOST_CLIENTHANDLER_HPP

#include "Client.hpp"

namespace ghost
{
	/**
	 * Base class for client handlers.
	 * @author	Mathieu Nassar
	 * @date	15.06.2018
	 */
	class ClientHandler
	{
	public:
		virtual ~ClientHandler() {}

		virtual bool handle(Client& client) = 0;
	};
}

#endif //GHOST_CLIENTHANDLER_HPP
