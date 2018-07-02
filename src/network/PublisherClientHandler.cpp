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
	_subscribers.push_back(client);

	return true;
}

bool PublisherClientHandler::send(const ghost::Message& message)
{
	std::lock_guard<std::mutex> lock(_subscribersMutex);

	auto it = _subscribers.begin();
	while (it != _subscribers.end())
	{
		if (!(*it)->send(message))
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
		(*it)->stop();
	}
	_subscribers.clear();
}
