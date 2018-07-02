#ifndef GHOST_INTERNAL_NETWORK_CLIENTMANAGER_HPP
#define GHOST_INTERNAL_NETWORK_CLIENTMANAGER_HPP

#include <deque>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>

#include "../../Client.hpp"

namespace ghost
{
	namespace internal
	{
		class ClientManager
		{
		public:
			ClientManager();
			~ClientManager();

			/// Starts a thread periodically deleting old clients
			void start();
			/// stops the thread periodically deleting old clients
			void stop();

			/// Adds a client to the manager
			void addClient(std::shared_ptr<ghost::Client> client);
			/// Releases a client, it will be deleted by the processing thread or by "deleteAllClients"
			void releaseClient(std::shared_ptr<ghost::Client> client);
			/// Deletes the clients that were released
			void deleteReleasedClients();
			
		private:
			/// Deletes all managed clients. Should not be called concurrently with "add"
			void deleteAllClients();
			void manageClients();

			std::thread _clientManagerThread;
			std::atomic<bool> _clientManagerThreadEnable;
			std::mutex _mutex;
			std::deque<std::shared_ptr<ghost::Client>> _allClients;
			std::deque<std::shared_ptr<ghost::Client>> _activeClientsList;
			std::deque<std::shared_ptr<ghost::Client>> _releasedClientsList;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_CLIENTMANAGER_HPP
