/*
 * Copyright 2020 Mathieu Nassar
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

#ifndef GHOST_CONNECTION_MANAGER_HPP
#define GHOST_CONNECTION_MANAGER_HPP

#include <ghost/connection/Client.hpp>
#include <ghost/connection/ConnectionConfiguration.hpp>
#include <ghost/connection/ConnectionFactory.hpp>
#include <ghost/connection/Publisher.hpp>
#include <ghost/connection/Server.hpp>
#include <ghost/connection/Subscriber.hpp>
#include <memory>

namespace ghost
{
/**
 * @brief Manager for Connection objects.
 *
 * The connection manager is responsible for the creation of connections and manages their lifetime.
 * Running connections will be terminated during the destruction of the connection manager.
 *
 * The connection manager includes a connection factory which creates the connections. It is possible
 * to add factory rules to create user-defined connections. See ConnectionFactory for more information.
 */
class ConnectionManager
{
public:
	static std::shared_ptr<ghost::ConnectionManager> create();
	virtual ~ConnectionManager() = default;

	/// Creates a server with the provided configuration. If this is impossible, returns nullptr.
	virtual std::shared_ptr<ghost::Server> createServer(const ghost::ConnectionConfiguration& config) = 0;
	/// Creates a client with the provided configuration. If this is impossible, returns nullptr.
	virtual std::shared_ptr<ghost::Client> createClient(const ghost::ConnectionConfiguration& config) = 0;
	/// Creates a publisher with the provided configuration. If this is impossible, returns nullptr.
	virtual std::shared_ptr<ghost::Publisher> createPublisher(const ghost::ConnectionConfiguration& config) = 0;
	/// Creates a subscriber with the provided configuration. If this is impossible, returns nullptr.
	virtual std::shared_ptr<ghost::Subscriber> createSubscriber(const ghost::ConnectionConfiguration& config) = 0;

	/**
	 * @brief Accesses the connection factory.
	 *
	 * @return the connection factory of this connection manager.
	 */
	virtual std::shared_ptr<ghost::ConnectionFactory> getConnectionFactory() = 0;
};
} // namespace ghost

#endif // GHOST_CONNECTION_MANAGER_HPP
