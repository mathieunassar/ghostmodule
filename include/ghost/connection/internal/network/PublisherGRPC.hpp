#ifndef GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP
#define GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP

#include <mutex>

#include <ghost/connection/Publisher.hpp>
#include <ghost/connection/internal/network/ServerGRPC.hpp>
#include <ghost/connection/internal/network/PublisherClientHandler.hpp>

namespace ghost
{
	namespace internal
	{
		class PublisherGRPC : public ghost::Publisher
		{
		public:
			PublisherGRPC(const ghost::ConnectionConfiguration& config);
			PublisherGRPC(const ghost::NetworkConnectionConfiguration& config);

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

		private:
			void writerThread(); // waits for the writer to be fed and sends the data to the handler

			ServerGRPC _server;
			std::shared_ptr<PublisherClientHandler> _handler;
			std::thread _writerThread;
			std::atomic_bool _writerThreadEnable;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP
