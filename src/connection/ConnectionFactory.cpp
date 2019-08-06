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

#include "../include/ghost/connection/internal/ConnectionFactory.hpp"
#include <ghost/connection/NetworkConnectionConfiguration.hpp>

#include <ghost/connection/internal/network/ServerGRPC.hpp>
#include <ghost/connection/internal/network/PublisherGRPC.hpp>
#include <ghost/connection/internal/network/SubscriberGRPC.hpp>
#include <ghost/connection/internal/network/ClientGRPC.hpp>

using namespace ghost::internal;

ConnectionFactory::ConnectionFactory()
{
	_internal = this;

	NetworkConnectionConfiguration config;
	addPublisherRule<internal::PublisherGRPC>(config);
	addSubscriberRule<internal::SubscriberGRPC>(config);
	addServerRule<internal::ServerGRPC>(config);
	addClientRule<internal::ClientGRPC>(config);
}

std::shared_ptr<ghost::Server> ConnectionFactory::createServer(const ghost::ConnectionConfiguration& config)
{
	for (auto it = _serverRules.begin(); it != _serverRules.end(); ++it)
	{
		if ((*it)->matches(config))
		{
			return std::dynamic_pointer_cast<ghost::Server>((*it)->create(config));
		}
	}
	return nullptr;
}

std::shared_ptr<ghost::Client> ConnectionFactory::createClient(const ghost::ConnectionConfiguration& config)
{
	for (auto it = _clientRules.begin(); it != _clientRules.end(); ++it)
	{
		if ((*it)->matches(config))
		{
			return std::dynamic_pointer_cast<ghost::Client>((*it)->create(config));
		}
	}
	return nullptr;
}

std::shared_ptr<ghost::Publisher> ConnectionFactory::createPublisher(const ghost::ConnectionConfiguration& config)
{
	for (auto it = _publisherRules.begin(); it != _publisherRules.end(); ++it)
	{
		if ((*it)->matches(config))
		{
			return std::dynamic_pointer_cast<ghost::Publisher>((*it)->create(config));
		}
	}
	return nullptr;
}

std::shared_ptr<ghost::Subscriber> ConnectionFactory::createSubscriber(const ghost::ConnectionConfiguration& config)
{
	for (auto it = _subscriberRules.begin(); it != _subscriberRules.end(); ++it)
	{
		if ((*it)->matches(config))
		{
			return std::dynamic_pointer_cast<ghost::Subscriber>((*it)->create(config));
		}
	}
	return nullptr;
}
