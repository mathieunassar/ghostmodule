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

#include "../../include/ghost/connection/internal/network/ClientGRPC.hpp"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "../../include/ghost/connection/internal/GenericMessageConverter.hpp"

using namespace ghost::internal;

ClientGRPC::ClientGRPC(const ghost::ConnectionConfiguration& config)
	: ClientGRPC(NetworkConnectionConfiguration::initializeFrom(config))
{

}

ClientGRPC::ClientGRPC(const ghost::NetworkConnectionConfiguration& config)
	: BaseClientGRPC(config, new grpc::CompletionQueue()) // Will be owned by the executor
	, _executor(_completionQueue) // now owns the completion queue
	, _initialized(false)
{
	_startedProcessor = std::bind(&ClientGRPC::onStarted, this, std::placeholders::_1);
	_finishProcessor = std::bind(&ClientGRPC::onFinished, this, std::placeholders::_1);

	_executor.start(_configuration.getThreadPoolSize());
}

/**
 *	Blocks until the client is connected thanks to the condition variable.
 */
bool ClientGRPC::start()
{
	if (!BaseClientGRPC::start())
		return false;

	std::ostringstream server_address;
	server_address << _configuration.getServerIpAddress() << ":" << _configuration.getServerPortNumber();

	startOperation();
	auto channel = grpc::CreateChannel(server_address.str(), grpc::InsecureChannelCredentials());
	_stub = protobuf::ServerClientService::NewStub(channel);

	std::unique_lock<std::mutex> lk(_initializedMutex);
	
	_client = _stub->Asyncconnect(_context.get(), _completionQueue, &_startedProcessor);
	
	_initializedConditionVariable.wait(lk, [this] {return _initialized; });

	return _statemachine.getState() == RPCStateMachine::EXECUTING; // could fail during statup
}

void ClientGRPC::onStarted(bool ok)
{
	if (!finishOperation())
		return;

	if (ok)
	{
		_statemachine.setState(RPCStateMachine::EXECUTING);
		startReader();
		if (isWriterConfigured())
			startWriter();
	}
	else
		_statemachine.setState(RPCStateMachine::FINISHED); // RPC could not start, finish it!

	_initialized = true;
	_initializedConditionVariable.notify_one();
}

bool ClientGRPC::stop()
{
	if (!BaseClientGRPC::stop())
	{
		return false;
	}

	grpc::Status status = grpc::Status::CANCELLED;
	if (_statemachine.getState() == RPCStateMachine::DISPOSING)
	{
		_context->TryCancel();
		startOperation();
		_client->Finish(&status, &_finishProcessor);
	}

	awaitFinished();
	_executor.stop();
	disposeGRPC();

	return status.ok();
}

void ClientGRPC::onFinished(bool ok) // OK ignored, the RPC is going to be deleted anyway
{
	finishOperation(); // don't care about the result, this method does not start more RPC operation

	_statemachine.setState(RPCStateMachine::FINISHED);
}
