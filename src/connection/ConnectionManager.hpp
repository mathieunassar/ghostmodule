/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GHOST_INTERNAL_CONNECTION_MANAGER_HPP
#define GHOST_INTERNAL_CONNECTION_MANAGER_HPP

#include <memory>

#include <ghost/connection/ConnectionManager.hpp>
#include "ConnectionFactory.hpp"

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
			void purgeDeadClients();

			std::shared_ptr<ghost::internal::ConnectionFactory> _connectionFactory;

			std::list<std::shared_ptr<ghost::Connection>> _connections;
		};
	}
}

#endif //GHOST_INTERNAL_CONNECTION_MANAGER_HPP
