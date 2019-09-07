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

#include "ClientManager.hpp"

#include "RemoteClientGRPC.hpp"

using namespace ghost::internal;

ClientManager::ClientManager()
{

}

ClientManager::~ClientManager()
{
	stop();

	deleteAllClients();
}

void ClientManager::start()
{
	_clientManagerThreadEnable = true;
	_clientManagerThread = std::thread(&ClientManager::manageClients, this);
}

void ClientManager::stop()
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (auto it = _allClients.begin(); it != _allClients.end(); ++it)
	{
		(*it)->stop();
	}

	_clientManagerThreadEnable = false;
	if (_clientManagerThread.joinable())
		_clientManagerThread.join();

	// do not delete the clients -> this might be called by a client's thread shutting down the server. In that case, its executor thread would try to join itself
}

void ClientManager::addClient(std::shared_ptr<RemoteClientGRPC> client)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_allClients.push_back(client);
}

void ClientManager::stopClients()
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (auto it = _allClients.begin(); it != _allClients.end(); ++it)
	{
		(*it)->stop();
	}
}

void ClientManager::deleteDisposableClients()
{
	std::lock_guard<std::mutex> lock(_mutex);
	auto it = _allClients.begin();
	while (it != _allClients.end())
	{
		// dispose and delete the client if it is finished and only owned by this
		if (!(*it)->isRunning() && it->use_count() == 1)
		{
			(*it)->getRPC()->dispose();
			it = _allClients.erase(it);
		}
		else
			++it;
	}
}

void ClientManager::deleteAllClients()
{
	for (auto it = _allClients.begin(); it != _allClients.end(); ++it)
	{
		(*it)->getRPC()->dispose();
	}
	_allClients.clear();
}

void ClientManager::manageClients()
{
	while (_clientManagerThreadEnable)
	{
		deleteDisposableClients(); // stop, dispose grpc and delete all clients whose shared_ptr has a counter of 1
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
