#ifndef GHOST_INTERNAL_NETWORK_SUBSCRIBERGRPC_HPP
#define GHOST_INTERNAL_NETWORK_SUBSCRIBERGRPC_HPP

#include "../../Subscriber.hpp"
#include "ClientGRPC.hpp"

namespace ghost
{
	namespace internal
	{
		template<typename MessageType>
		class SubscriberGRPC : public ghost::Subscriber<MessageType>
		{
		public:
			SubscriberGRPC(const ghost::NetworkConnectionConfiguration& config);

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

			void setMessageHandler(std::function<void(const MessageType& message)> handler) override;

		private:
			ClientGRPC _client;
		};

		// template definition
		#include "SubscriberGRPC.impl.hpp"
	}
}

#endif // GHOST_INTERNAL_NETWORK_SUBSCRIBERCOREGRPC_HPP
