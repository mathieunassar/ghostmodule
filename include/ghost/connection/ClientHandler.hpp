#ifndef GHOST_CLIENTHANDLER_HPP
#define GHOST_CLIENTHANDLER_HPP

#include <ghost/connection/Client.hpp>

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
		virtual ~ClientHandler() = 0;

		virtual bool handle(std::shared_ptr<Client> client, bool& keepClientAlive) = 0;
	};

	inline ClientHandler::~ClientHandler() {}
}

#endif //GHOST_CLIENTHANDLER_HPP
