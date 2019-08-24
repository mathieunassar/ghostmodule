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

#ifndef GHOST_CONNECTIONGRPC_HPP
#define GHOST_CONNECTIONGRPC_HPP

#include <ghost/connection/ConnectionManager.hpp>

namespace ghost
{
	/**
	 *	Manages the ghost_connection_grpc content.
	 *	Use the "initialize" method to load new rules for the connection manager's factory.
	 *	To use GRPC as a communication technology in the connection library, use the create methods
	 *	of the ghost::ConnectionManager after this initialization with configurations of type
	 *	ghost::NetworkConnectionConfiguration.
	 */
	class ConnectionGRPC
	{
		/**
		 *	Loads factory rules into the given connection manager objects to enable the creation
		 *	of gRPC-based connections.
		 *	@params connectionManager	the connection manager used by this program.
		 */
		static void initialize(const std::shared_ptr<ghost::ConnectionManager>& connectionManager);
	};
}

#endif // GHOST_CONNECTIONGRPC_HPP