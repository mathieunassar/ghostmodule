#ifndef GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP
#define GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP

#include <mutex>
#include <deque>
#include <iostream>

#include "../../ClientHandler.hpp"
#include "../../Writer.hpp"

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
			
			bool send(const google::protobuf::Any& message);
			void releaseClients();

		private:
			std::mutex _subscribersMutex;
			std::deque<	std::pair<	std::shared_ptr<ghost::Client>,
									std::shared_ptr<Writer<google::protobuf::Any>>>> _subscribers;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP
