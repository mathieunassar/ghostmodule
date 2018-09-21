#ifndef GHOST_INTERNAL_NETWORK_CLIENTMANAGER_HPP
#define GHOST_INTERNAL_NETWORK_CLIENTMANAGER_HPP

#include <deque>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>

namespace ghost
{
	namespace internal
	{
		class RemoteClientGRPC;

		class ClientManager
		{
		public:
			ClientManager();
			~ClientManager();

			/// Starts a thread periodically deleting old clients
			void start();
			/// stops the thread periodically deleting old clients, and deltes all the clients after disposing them
			/// this call might be blocking while the clients are disposing
			void stop();

			/// Adds a client to the manager
			void addClient(std::shared_ptr<RemoteClientGRPC> client);
			/// dispose and delete clients that are in finished state and owned solely by this manager
			void deleteDisposableClients();
			
		private:
			/// Deletes all managed clients. Should not be called concurrently with "add"
			void deleteAllClients();
			/// loops over the managed clients and tries to delte unused clients with "deleteDisposableClients"
			void manageClients();

			std::thread _clientManagerThread;
			std::atomic<bool> _clientManagerThreadEnable;
			std::mutex _mutex;
			std::deque<std::shared_ptr<RemoteClientGRPC>> _allClients;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_CLIENTMANAGER_HPP
