#include "../include/internal/ConnectionManager.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::ConnectionManager> ghost::ConnectionManager::create()
{
	return std::make_shared<ghost::internal::ConnectionManager>();
}

ConnectionManager::ConnectionManager()
	: _connectionFactory(std::make_shared<ghost::internal::ConnectionFactory>())
{
	_internal = this;
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
	return _connectionFactory->createServer(config);
}

std::shared_ptr<ghost::Client> ConnectionManager::createClient(const ghost::ConnectionConfiguration& config)
{
	return _connectionFactory->createClient(config);
}

std::shared_ptr<ghost::Publisher> ConnectionManager::createPublisher(const ghost::ConnectionConfiguration& config)
{
	return _connectionFactory->createPublisher(config);
}

std::shared_ptr<ghost::Subscriber> ConnectionManager::createSubscriber(const ghost::ConnectionConfiguration& config)
{
	return _connectionFactory->createSubscriber(config);
}

std::shared_ptr<ghost::ConnectionFactory> ConnectionManager::getConnectionFactory()
{
	return _connectionFactory;
}
