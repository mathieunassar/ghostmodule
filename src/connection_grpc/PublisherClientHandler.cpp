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

#include "PublisherClientHandler.hpp"

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
		if (!it->first->isRunning() // if the client is not running anymore, dont send anything
			|| !it->second->write(message)) // if the write failed
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
