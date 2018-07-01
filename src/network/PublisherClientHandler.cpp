#include "../../include/internal/network/PublisherClientHandler.hpp"
#include <iostream>

using namespace ghost::internal;

bool PublisherClientHandler::handle(ghost::Client& client, bool& keepClientAlive)
{
	keepClientAlive = true;

	std::lock_guard<std::mutex> lock(_subscribersMutex);
	_subscribers.push_back(&client);

	return true;
}

bool PublisherClientHandler::send(const ghost::Message& message)
{
	std::lock_guard<std::mutex> lock(_subscribersMutex);
	std::cout << _subscribers.size() << " subscribers" << std::endl;
	for (auto it = _subscribers.begin(); it != _subscribers.end(); ++it)
	{
		if (!(*it)->send(message))
		{
			std::cout << "a client disconnected" << std::endl;
		}
	}
	return true;
}
