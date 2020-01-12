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

#ifndef GHOST_CONNECTIONCONFIGURATIONGRPC_HPP
#define GHOST_CONNECTIONCONFIGURATIONGRPC_HPP

#include <ghost/connection/NetworkConnectionConfiguration.hpp>

namespace ghost
{
/**
 * @brief Extended connection configuration for network connections using gRPC.
 * This configuration possesses an additional attribute that allows the ghost::ConnectionFactory
 * to differentiate gRPC network connections from other network connection technologies.
 */
class ConnectionConfigurationGRPC : public ghost::NetworkConnectionConfiguration
{
public:
	/**
	 * @brief Constructs a new NetworkConnectionConfiguration object with
	 * default parameters, i.e. any IP address and any remote port number.
	 *
	 * @param name the name of the configuration
	 */
	ConnectionConfigurationGRPC(const std::string& name = "");
	ConnectionConfigurationGRPC(const std::string& ip, int port);
};
} // namespace ghost

#endif // GHOST_CONNECTIONCONFIGURATIONGRPC_HPP
