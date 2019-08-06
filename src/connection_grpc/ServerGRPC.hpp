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

#ifndef GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP
#define GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP

#include <functional>
#include <atomic>

#include <grpcpp/server.h>
#include "../../../../../protobuf/Ghost.pb.h"
#include "../../../../../protobuf/Ghost.grpc.pb.h"

#include <ghost/connection/Server.hpp>
#include <ghost/connection/NetworkConnectionConfiguration.hpp>
#include <ghost/connection/internal/network/CompletionQueueExecutor.hpp>
#include <ghost/connection/internal/network/ClientManager.hpp>

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
			ServerGRPC(const ghost::ConnectionConfiguration& config);
			ServerGRPC(const ghost::NetworkConnectionConfiguration& config);
			~ServerGRPC();

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

			void setClientHandler(std::shared_ptr<ClientHandler> handler) override;

		private:
			ghost::NetworkConnectionConfiguration _configuration;
			std::atomic<bool> _running;

			protobuf::ServerClientService::AsyncService _service;
			std::unique_ptr<grpc::Server> _grpcServer;
			CompletionQueueExecutor _completionQueueExecutor;
			
			ClientManager _clientManager;
			std::shared_ptr<ClientHandler> _clientHandler;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP
