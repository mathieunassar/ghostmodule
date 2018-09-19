#ifndef GHOST_CONNECTION_MANAGER_HPP
#define GHOST_CONNECTION_MANAGER_HPP

#include <memory>

#include "Server.hpp"
#include "Client.hpp"
#include "Publisher.hpp"
#include "Subscriber.hpp"
#include "ConnectionConfiguration.hpp"
#include "ConnectionFactory.hpp"

namespace ghost
{
	namespace internal
	{
		class ConnectionManager;
	}

	/**
	 *	The connection manager is responsible for the creation of connections and manages their lifetime.
	 *	Running connections will be terminated during the destruction of the connection manager.
	 */
	class ConnectionManager
	{
	public:
		static std::shared_ptr<ConnectionManager> create();
		virtual ~ConnectionManager() = 0 {}

		/// Creates a server with the provided configuration. If this is impossible, returns nullptr.
		virtual std::shared_ptr<Server> createServer(const ConnectionConfiguration& config) = 0;
		/// Creates a client with the provided configuration. If this is impossible, returns nullptr.
		virtual std::shared_ptr<Client> createClient(const ConnectionConfiguration& config) = 0;
		/// Creates a publisher with the provided configuration. If this is impossible, returns nullptr.
		virtual std::shared_ptr<Publisher> createPublisher(const ConnectionConfiguration& config) = 0;
		/// Creates a subscriber with the provided configuration. If this is impossible, returns nullptr.
		virtual std::shared_ptr<Subscriber> createSubscriber(const ConnectionConfiguration& config) = 0;

		virtual std::shared_ptr<ghost::ConnectionFactory> getConnectionFactory() = 0;

	protected:
		internal::ConnectionManager* _internal;
	};
}

#include "internal/ConnectionManager.hpp"

#endif //GHOST_CONNECTION_MANAGER_HPP
