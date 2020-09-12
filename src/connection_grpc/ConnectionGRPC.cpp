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

#include <ghost/connection_grpc/ConnectionConfigurationGRPC.hpp>
#include <ghost/connection_grpc/ConnectionGRPC.hpp>
#include "ConnectionFactoryRuleGRPC.hpp"

#include "ClientGRPC.hpp"
#include "PublisherGRPC.hpp"
#include "ServerGRPC.hpp"
#include "SubscriberGRPC.hpp"

using namespace ghost;

void blackholeLogger(gpr_log_func_args* args)
{
}

namespace
{
std::shared_ptr<ghost::ThreadPool> connectionGRPCThreadPool;
}

// Template specialization for the ghost::ConnectionFactory

template <>
void ghost::ConnectionFactory::addServerRule<ghost::internal::ServerGRPC>(const ghost::ConnectionConfiguration& config)
{
	return addServerRule(std::make_shared<internal::ConnectionFactoryRuleGRPC<ghost::internal::ServerGRPC>>(
	    config, connectionGRPCThreadPool));
}

template <>
void ghost::ConnectionFactory::addClientRule<ghost::internal::ClientGRPC>(const ghost::ConnectionConfiguration& config)
{
	return addClientRule(std::make_shared<internal::ConnectionFactoryRuleGRPC<ghost::internal::ClientGRPC>>(
	    config, connectionGRPCThreadPool));
}

template <>
void ghost::ConnectionFactory::addPublisherRule<ghost::internal::PublisherGRPC>(
    const ghost::ConnectionConfiguration& config)
{
	return addPublisherRule(std::make_shared<internal::ConnectionFactoryRuleGRPC<ghost::internal::PublisherGRPC>>(
	    config, connectionGRPCThreadPool));
}

template <>
void ghost::ConnectionFactory::addSubscriberRule<ghost::internal::SubscriberGRPC>(
    const ghost::ConnectionConfiguration& config)
{
	return addSubscriberRule(std::make_shared<internal::ConnectionFactoryRuleGRPC<ghost::internal::SubscriberGRPC>>(
	    config, connectionGRPCThreadPool));
}

void ConnectionGRPC::initialize(const std::shared_ptr<ghost::ConnectionManager>& connectionManager,
				const std::shared_ptr<ghost::ThreadPool>& threadPool,
				const ghost::NetworkConnectionConfiguration& minimumConfiguration)
{
	gpr_set_log_function(blackholeLogger);

	connectionGRPCThreadPool = threadPool;

	// Assign the gRPC implementations to this configuration.
	connectionManager->getConnectionFactory()->addServerRule<internal::ServerGRPC>(minimumConfiguration);
	connectionManager->getConnectionFactory()->addClientRule<internal::ClientGRPC>(minimumConfiguration);
	connectionManager->getConnectionFactory()->addPublisherRule<internal::PublisherGRPC>(minimumConfiguration);
	connectionManager->getConnectionFactory()->addSubscriberRule<internal::SubscriberGRPC>(minimumConfiguration);

	connectionGRPCThreadPool.reset();
}
