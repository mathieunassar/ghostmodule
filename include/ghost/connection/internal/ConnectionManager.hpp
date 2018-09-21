#ifndef GHOST_INTERNAL_CONNECTION_MANAGER_HPP
#define GHOST_INTERNAL_CONNECTION_MANAGER_HPP

#include <memory>

#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection/internal/ConnectionFactory.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		*	The connection manager is responsible for the creation of connections and manages their lifetime.
		*	Running connections will be terminated during the destruction of the connection manager.
		*/
		class ConnectionManager : public ghost::ConnectionManager
		{
		public:
			ConnectionManager();
			~ConnectionManager();

			/* connection creation methods */
			std::shared_ptr<ghost::Server> createServer(const ghost::ConnectionConfiguration& config) override;
			std::shared_ptr<ghost::Client> createClient(const ghost::ConnectionConfiguration& config) override;
			std::shared_ptr<ghost::Publisher> createPublisher(const ghost::ConnectionConfiguration& config) override;
			std::shared_ptr<ghost::Subscriber> createSubscriber(const ghost::ConnectionConfiguration& config) override;

			std::shared_ptr<ghost::ConnectionFactory> getConnectionFactory() override;

		private:
			std::shared_ptr<ghost::internal::ConnectionFactory> _connectionFactory;

			std::list<std::shared_ptr<ghost::Connection>> _connections;
		};
	}
}

#endif //GHOST_INTERNAL_CONNECTION_MANAGER_HPP
