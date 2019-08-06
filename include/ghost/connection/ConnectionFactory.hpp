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

#ifndef GHOST_CONNECTIONFACTORY_HPP
#define GHOST_CONNECTIONFACTORY_HPP

#include <ghost/connection/ConnectionConfiguration.hpp>

namespace ghost
{
	namespace internal
	{
		class ConnectionFactory;
	}

	/**
	 * @brief Factory that creates Connection objects based on a connection configuration.
	 * 
	 * The factory contains connection types mapped to minimal connection configurations.
	 * When creating connections based on a connection configuration, the factory searches
	 * for the first entry in the map, for which the given configuration matches the minimal
	 * required configuration.
	 * 
	 * For example, the default implementation of the connection manager adds factory rules
	 * for network based connections. The minimum configuration for such connections contain
	 * a server IP address and a remote port number (note that this minimal configuration can
	 * be created by the default constructor of NetworkConnectionConfiguration). Hence,
	 * attempting to create a connection with a NetworkConnectionConfiguration will match
	 * these factory rules.
	 * 
	 * The public interface of the connection factory only allows the user to add more rules.
	 * Creating connections using the factory can only be done through the ConnectionManager.
	 */
	class ConnectionFactory
	{
	public:
		/**
		 * @brief Adds a rule for a Server Connection.
		 * 
		 * @tparam Connection User-defined implementation of a server
		 * @param config Minimum configuration required to match this rule.
		 */
		template<typename Connection>
		void addServerRule(const ghost::ConnectionConfiguration& config);

		/**
		 * @brief Adds a rule for a Client Connection.
		 * 
		 * @tparam Connection User-defined implementation of a client
		 * @param config Minimum configuration required to match this rule.
		 */
		template<typename Connection>
		void addClientRule(const ghost::ConnectionConfiguration& config);

		/**
		 * @brief Adds a rule for a Publisher Connection.
		 * 
		 * @tparam Connection User-defined implementation of a publisher
		 * @param config Minimum configuration required to match this rule.
		 */
		template<typename Connection>
		void addPublisherRule(const ghost::ConnectionConfiguration& config);

		/**
		 * @brief Adds a rule for a Subscriber Connection.
		 * 
		 * @tparam Connection User-defined implementation of a subscriber
		 * @param config Minimum configuration required to match this rule.
		 */
		template<typename Connection>
		void addSubscriberRule(const ghost::ConnectionConfiguration& config);

	protected:
		internal::ConnectionFactory* _internal;
	};
}

#endif //GHOST_CONNECTIONFACTORY_HPP
