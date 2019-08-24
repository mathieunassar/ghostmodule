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

#include <ghost/connection_grpc/ConnectionGRPC.hpp>
#include "ServerGRPC.hpp"
#include "PublisherGRPC.hpp"
#include "ClientGRPC.hpp"
#include "SubscriberGRPC.hpp"

using namespace ghost;

void initialize(const std::shared_ptr<ghost::ConnectionManager>& connectionManager)
{
	// This is the minimal configuration for gRPC-based connections.
	ghost::NetworkConnectionConfiguration config;

	// Assign the gRPC implementations to this configuration.
	connectionManager->getConnectionFactory()->addServerRule<internal::ServerGRPC>(config);
	connectionManager->getConnectionFactory()->addClientRule<internal::ClientGRPC>(config);
	connectionManager->getConnectionFactory()->addPublisherRule<internal::PublisherGRPC>(config);
	connectionManager->getConnectionFactory()->addSubscriberRule<internal::SubscriberGRPC>(config);
}
