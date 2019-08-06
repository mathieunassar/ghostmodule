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

#include "../../include/ghost/connection/internal/network/ServerGRPC.hpp"

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "../../include/ghost/connection/internal/network/RemoteClientGRPC.hpp"
#include "../../include/ghost/connection/internal/network/ServerGRPC.hpp"

using namespace ghost::internal;

ServerGRPC::ServerGRPC(const ghost::ConnectionConfiguration& config)
	: ServerGRPC(ghost::NetworkConnectionConfiguration::initializeFrom(config))
{

}

ServerGRPC::ServerGRPC(const ghost::NetworkConnectionConfiguration& config)
	: Server(config)
	, _configuration(config)
	, _running(false)
{

}

ServerGRPC::~ServerGRPC()
{
	
}

bool ServerGRPC::start()
{
	if (_running)
		return false;

	_running = true;

	std::ostringstream server_address;
	server_address << _configuration.getServerIpAddress() << ":" << _configuration.getServerPortNumber();

	grpc::ServerBuilder builder;

	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address.str(), ::grpc::InsecureServerCredentials());

	// Register "_service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *asynchronous* service.
	builder.RegisterService(&_service);

	// Get hold of the completion queue used for the asynchronous communication
	// with the gRPC runtime.
	_completionQueueExecutor.setCompletionQueue(builder.AddCompletionQueue());

	// Finally assemble the server.
	_grpcServer = builder.BuildAndStart();

	_completionQueueExecutor.start(_configuration.getThreadPoolSize());

	auto cq = static_cast<grpc::ServerCompletionQueue*>(_completionQueueExecutor.getCompletionQueue());
	for (size_t i = 0; i < _configuration.getThreadPoolSize(); i++) // start as many calls as there can be concurrent rpcs
	{
		// Spawn a new CallData instance to serve new clients
		auto client = std::make_shared<RemoteClientGRPC>(_configuration, &_service, cq, _clientHandler, &_clientManager, this);
		_clientManager.addClient(client);
	}
	_clientManager.start();

	return true;
}

bool ServerGRPC::stop()
{
	if (!_running)
		return false;

	_running = false;

	_grpcServer->Shutdown();
	_completionQueueExecutor.stop(); // shutdowns the completion queue

	_clientManager.stop();

	return true;
}

bool ServerGRPC::isRunning() const
{
	return _running;
}

void ServerGRPC::setClientHandler(std::shared_ptr<ghost::ClientHandler> handler)
{
	_clientHandler = handler;
}
