#include "../../include/internal/network/PublisherClientHandler.hpp"
#include <iostream>

using namespace ghost::internal;

PublisherClientHandler::~PublisherClientHandler()
{
	releaseClients();
}

bool PublisherClientHandler::handle(std::shared_ptr<ghost::Client> client, bool& keepClientAlive)
{
	keepClientAlive = true;

	std::lock_guard<std::mutex> lock(_subscribersMutex);

	auto writer = client->getWriter<google::protobuf::Any>();
	auto entry = std::make_pair(client, writer);

	_subscribers.push_back(entry);

	return true;
}

bool PublisherClientHandler::send(const google::protobuf::Any& message)
{
	std::lock_guard<std::mutex> lock(_subscribersMutex);

	auto it = _subscribers.begin();
	while (it != _subscribers.end())
	{
		if (!it->second->write(message))
		{
			it = _subscribers.erase(it);
		}
		else
			++it;
	}
	
	return true;
}

void PublisherClientHandler::releaseClients()
{
	std::lock_guard<std::mutex> lock(_subscribersMutex);
	for (auto it = _subscribers.begin(); it != _subscribers.end(); ++it)
	{
		it->first->stop();
	}
	_subscribers.clear();
}
