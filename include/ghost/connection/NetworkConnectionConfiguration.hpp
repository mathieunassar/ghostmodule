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

#ifndef GHOST_NETWORKCONNECTIONCONFIGURATION_HPP
#define GHOST_NETWORKCONNECTIONCONFIGURATION_HPP

#include <string>
#include <ghost/connection/ConnectionConfiguration.hpp>

namespace ghost
{
	/**
	 * @brief Extended connection configuration for network connections.
	 * 
	 * This connection configuration contains additional required parameters
	 * for network connections:
	 * - The IP address of the target connection
	 * - the port number to reach.
	 */
	class NetworkConnectionConfiguration : public ghost::ConnectionConfiguration
	{
	public:
		/**
		 * @brief Constructs a new NetworkConnectionConfiguration object with
		 * default parameters, i.e. any IP address and any remote port number.
		 * 
		 * @param name the name of the configuration
		 */
		NetworkConnectionConfiguration(const std::string& name = "");
		virtual ~NetworkConnectionConfiguration() = default;
		 
		/**
		 * @brief Accessor for the IP address of the target connection
		 * 
		 * @return the requested IP address
		 */
		std::string getServerIpAddress() const;
		/**
		 * @brief Accessor for the port number of the target connection
		 * 
		 * @return the requested port number
		 */
		int getServerPortNumber() const;

		/**
		 * @brief Set the IP address of the target connection
		 * 
		 * @param ip the new IP address to set
		 */
		void setServerIpAddress(const std::string& ip);
		/**
		 * @brief Set the server port number of the target connection
		 * 
		 * @param port the new port number to set
		 */
		void setServerPortNumber(int port);

		/**
		 * @brief Creates a network connection configuration from a connection
		 * configuration
		 * 
		 * @param from source connection configuration
		 * 
		 * @return NetworkConnectionConfiguration the created network connection configuration
		 */
		static ghost::NetworkConnectionConfiguration initializeFrom(const ghost::ConnectionConfiguration& from);
	};
}

#endif //GHOST_CONNECTIONCONFIGURATION_HPP
