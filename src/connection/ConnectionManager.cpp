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

#include "ConnectionManager.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::ConnectionManager> ghost::ConnectionManager::create()
{
	return std::make_shared<ghost::internal::ConnectionManager>();
}

ConnectionManager::ConnectionManager()
	: _connectionFactory(std::make_shared<ghost::internal::ConnectionFactory>())
{
}

ConnectionManager::~ConnectionManager()
{
	for (auto& conn : _connections)
	{
		if (conn->isRunning())
		{
			conn->stop();
		}
	}
}

std::shared_ptr<ghost::Server> ConnectionManager::createServer(const ghost::ConnectionConfiguration& config)
{
	purgeDeadClients();
	auto conn = _connectionFactory->createServer(config);
	if (conn)
		_connections.push_back(conn);
	return conn;
}

std::shared_ptr<ghost::Client> ConnectionManager::createClient(const ghost::ConnectionConfiguration& config)
{
	purgeDeadClients();
	auto conn = _connectionFactory->createClient(config);
	if (conn)
		_connections.push_back(conn);
	return conn;
}

std::shared_ptr<ghost::Publisher> ConnectionManager::createPublisher(const ghost::ConnectionConfiguration& config)
{
	purgeDeadClients();
	auto conn = _connectionFactory->createPublisher(config);
	if (conn)
		_connections.push_back(conn);
	return conn;
}

std::shared_ptr<ghost::Subscriber> ConnectionManager::createSubscriber(const ghost::ConnectionConfiguration& config)
{
	purgeDeadClients();
	auto conn = _connectionFactory->createSubscriber(config);
	if (conn)
		_connections.push_back(conn);
	return conn;
}

std::shared_ptr<ghost::ConnectionFactory> ConnectionManager::getConnectionFactory()
{
	return _connectionFactory;
}

void ConnectionManager::purgeDeadClients()
{
	auto it = _connections.begin();
	while (it != _connections.end())
	{
		if (!(*it)->isRunning() && it->use_count() == 1)
			it = _connections.erase(it);
		else
			++it;
	}
}
