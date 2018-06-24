#include "../../include/internal/network/ClientGRPC.hpp"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "../../include/internal/GenericMessageConverter.hpp"

using namespace ghost::internal;

ClientGRPC::ClientGRPC(const ghost::NetworkConnectionConfiguration& config)
	: BaseClientGRPC(config, new grpc::CompletionQueue()) // Will be owned by the executor
	, _initialized(false)
	, _executor(_completionQueue) // now owny the completion queue
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
		startReader();
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

	startOperation();
	grpc::Status status;
	_client->Finish(&status, &_finishProcessor);

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