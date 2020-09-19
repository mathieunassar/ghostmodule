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

#include <ghost/connection_grpc/ServerClientService.grpc.pb.h>
#include <ghost/connection_grpc/ServerClientService.pb.h>
#include <grpcpp/server.h>

#include <atomic>
#include <ghost/connection/NetworkConnectionConfiguration.hpp>
#include <ghost/connection/Server.hpp>
#include <ghost/module/ThreadPool.hpp>
#include <memory>

#include "ClientManager.hpp"
#include "CompletionQueueExecutor.hpp"

namespace ghost
{
namespace internal
{
class IncomingRPC;

/**
 * Server implementation using the gRPC library. Runs a gRPC server which accepts connections, and create
 * a writing/sending interface which is returned to the server object.
 */
class ServerGRPC : public ghost::Server
{
public:
	ServerGRPC(const ghost::ConnectionConfiguration& config, const std::shared_ptr<ghost::ThreadPool>& threadPool);
	ServerGRPC(const ghost::NetworkConnectionConfiguration& config,
		   const std::shared_ptr<ghost::ThreadPool>& threadPool);

	bool start() override;
	bool stop() override;
	bool isRunning() const override;
	bool isShutdown() const;

	void shutdown();
	void setClientHandler(std::shared_ptr<ClientHandler> handler) override;
	const std::shared_ptr<ClientHandler> getClientHandler() const;

private:
	void onClientConnected(std::shared_ptr<RemoteClientGRPC> client);

	std::shared_ptr<ghost::ThreadPool> _threadPool;
	ghost::NetworkConnectionConfiguration _configuration;
	std::atomic<bool> _running;

	ghost::protobuf::connectiongrpc::ServerClientService::AsyncService _service;
	std::unique_ptr<grpc::Server> _grpcServer;
	CompletionQueueExecutor _completionQueueExecutor;

	ClientManager _clientManager;
	std::shared_ptr<ClientHandler> _clientHandler;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP
