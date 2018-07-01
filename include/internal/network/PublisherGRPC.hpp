#ifndef GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP
#define GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP

#include <mutex>

#include "../../Publisher.hpp"
#include "ServerGRPC.hpp"
#include "PublisherClientHandler.hpp"

namespace ghost
{
	namespace internal
	{

		template<typename MessageType>
		class PublisherGRPC : public ghost::Publisher<MessageType>
		{
		public:
			PublisherGRPC(const ghost::NetworkConnectionConfiguration& config);

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

			void publish(const MessageType& message) override;

		private:
			ServerGRPC _server;
			std::shared_ptr<PublisherClientHandler> _handler;
		};

		// specialization for ghost::Message to serialize it first into protobuf
		template<>
		void PublisherGRPC<ghost::Message>::publish(const ghost::Message& message);

		// template definition
		#include "PublisherGRPC.impl.hpp"
	}
}

#endif // GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP
