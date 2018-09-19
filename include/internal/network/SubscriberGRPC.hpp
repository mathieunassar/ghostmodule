#ifndef GHOST_INTERNAL_NETWORK_SUBSCRIBERGRPC_HPP
#define GHOST_INTERNAL_NETWORK_SUBSCRIBERGRPC_HPP

#include "../../Subscriber.hpp"
#include "ClientGRPC.hpp"

namespace ghost
{
	namespace internal
	{
		class SubscriberGRPC : public ghost::Subscriber
		{
		public:
			SubscriberGRPC(const ghost::ConnectionConfiguration& config);
			SubscriberGRPC(const ghost::NetworkConnectionConfiguration& config);

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

		private:
			/// From internal::Connection - forwards the sink to the client
			virtual void setReaderSink(std::shared_ptr<ReaderSink> sink);
			ghost::NetworkConnectionConfiguration makeNoWriterConfig(const ghost::NetworkConnectionConfiguration& config);

			ClientGRPC _client;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_SUBSCRIBERCOREGRPC_HPP
