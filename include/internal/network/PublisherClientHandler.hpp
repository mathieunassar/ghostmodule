#ifndef GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP
#define GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP

#include <mutex>
#include <deque>
#include <iostream>

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
			~PublisherClientHandler();

			bool handle(std::shared_ptr<ghost::Client> client, bool& keepClientAlive) override;
			
			bool send(const ghost::Message& message);
			void releaseClients();

		private:
			std::mutex _subscribersMutex;
			std::deque<std::shared_ptr<ghost::Client>> _subscribers;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP
