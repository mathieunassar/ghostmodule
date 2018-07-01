#ifndef GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP
#define GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP

#include <mutex>
#include <deque>

#include "../../ClientHandler.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		 *	This handler keeps the clients which connect to the server, and sends them the published data.
		 */
		class PublisherClientHandler : public ghost::ClientHandler
		{
		public:
			bool handle(ghost::Client& client, bool& keepClientAlive) override;
			
			bool send(const ghost::Message& message);

		private:
			std::mutex _subscribersMutex;
			std::deque<ghost::Client*> _subscribers;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP
