#ifndef GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP
#define GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP

#include <functional>
#include <thread>

#include <grpcpp/server.h>
#include "../../../protobuf/Ghost.pb.h"
#include "../../../protobuf/Ghost.grpc.pb.h"

#include "../../Server.hpp"
#include "../../NetworkConnectionConfiguration.hpp"
#include "CompletionQueueExecutor.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		 * Server implementation using the gRPC library. Runs a gRPC server which accepts connections, and create
		 * a writing/sending interface which is returned to the server object.
		 * @author	Mathieu Nassar
		 * @date	25.05.2018
		 */
		class ServerGRPC : public ghost::Server
		{
		public:
			ServerGRPC(const ghost::NetworkConnectionConfiguration& config);

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

			void setClientHandler(std::shared_ptr<ClientHandler> handler) override;

		private:
			ghost::NetworkConnectionConfiguration _configuration;

			protobuf::ServerClientService::AsyncService _service;
			std::unique_ptr<grpc::Server> _grpcServer;
			CompletionQueueExecutor _completionQueueExecutor;
			
			std::shared_ptr<ClientHandler> _clientHandler;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP
