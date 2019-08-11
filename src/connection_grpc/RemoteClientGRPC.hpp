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

#ifndef GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP

#include <thread>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "BaseClientGRPC.hpp"
#include "ClientManager.hpp"
#include <ghost/connection/Server.hpp>
#include <ghost/connection/ClientHandler.hpp>

namespace ghost
{
	namespace internal
	{
		class RemoteClientGRPC : public BaseClientGRPC<grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>, grpc::ServerContext>
		{
		public:
			RemoteClientGRPC(const NetworkConnectionConfiguration& config,
				ghost::protobuf::connectiongrpc::ServerClientService::AsyncService* service, grpc::ServerCompletionQueue* completionQueue,
				std::shared_ptr<ClientHandler> callback,
				ClientManager* clientManager,
				ghost::Server* server);
			~RemoteClientGRPC();

			bool start() override;
			bool stop() override;

			void dispose();

		protected:
			void onStarted(bool ok);
			void onFinished(bool ok);
			void onDone(bool ok);
			void execute();

			std::function<void(bool)> _startedProcessor;
			std::function<void(bool)> _finishProcessor;
			std::function<void(bool)> _doneProcessor;
			ghost::protobuf::connectiongrpc::ServerClientService::AsyncService* _service;
			std::shared_ptr<ClientHandler> _clientHandler;
			std::thread _executionThread;

			ClientManager* _clientManager;
			ghost::Server* _server;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
