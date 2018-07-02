#include "../../include/internal/network/ClientManager.hpp"

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
	for (auto it = _activeClientsList.begin(); it != _activeClientsList.end(); ++it)
	{
		(*it)->stop();
	}

	_clientManagerThreadEnable = false;
	if (_clientManagerThread.joinable())
		_clientManagerThread.join();
}

void ClientManager::deleteReleasedClients()
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (auto it = _releasedClientsList.begin(); it != _releasedClientsList.end(); ++it)
	{
		// 1. search and remove the client from the total list
		for (auto it2 = _allClients.begin(); it2 != _allClients.end(); ++it2)
		{
			if ((*it2) == (*it))
			{
				_allClients.erase(it2);
				break;
			}
		}
	}
	_releasedClientsList.clear();
}

void ClientManager::deleteAllClients()
{
	_releasedClientsList.clear();
	_activeClientsList.clear();
	_allClients.clear();
}

void ClientManager::addClient(std::shared_ptr<ghost::Client> client)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_activeClientsList.push_back(client);
	_allClients.push_back(client);
}

void ClientManager::releaseClient(std::shared_ptr<ghost::Client> client)
{
	std::lock_guard<std::mutex> lock(_mutex);
	for (auto it = _activeClientsList.begin(); it != _activeClientsList.end(); ++it)
	{
		if (*it == client)
		{
			_releasedClientsList.push_back(client);
			_activeClientsList.erase(it);
			return;
		}
	}
}

void ClientManager::manageClients()
{
	while (_clientManagerThreadEnable)
	{
		deleteReleasedClients();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
