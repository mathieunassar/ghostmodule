#include "../../include/ghost/connection/internal/network/ClientManager.hpp"

#include "../../include/ghost/connection/internal/network/RemoteClientGRPC.hpp"

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

	deleteAllClients();
}

void ClientManager::addClient(std::shared_ptr<RemoteClientGRPC> client)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_allClients.push_back(client);
}

void ClientManager::deleteDisposableClients()
{
	std::lock_guard<std::mutex> lock(_mutex);
	auto it = _allClients.begin();
	while (it != _allClients.end())
	{
		// dispose and delete the client if it is finished and only owned by this
		if ((*it)->isFinished() && it->use_count() == 1)
		{
			(*it)->dispose();
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
		(*it)->dispose();
	}
	_allClients.clear();
}

void ClientManager::manageClients()
{
	while (_clientManagerThreadEnable)
	{
		deleteDisposableClients(); // stop, dispose grpc and delete all clients whose shared_ptr has a counter of 1
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
