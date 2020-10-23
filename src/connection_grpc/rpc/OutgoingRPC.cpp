/*
 * Copyright 2020 Mathieu Nassar
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

#include "OutgoingRPC.hpp"

#include <grpc/grpc.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "RPCConnect.hpp"
#include "RPCFinish.hpp"

using namespace ghost::internal;

OutgoingRPC::OutgoingRPC(const std::shared_ptr<ghost::ThreadPool>& threadPool, const std::string& serverIp,
			 int serverPort, size_t dedicatedThreads)
    : WriterRPC(threadPool)
    , _threadPool(threadPool)
    , _completionQueue(new grpc::CompletionQueue()) // Will be owned by the executor
    , _serverIp(serverIp)
    , _serverPort(serverPort)
    , _rpc(std::make_shared<RPC<ReaderWriter, ContextType>>(threadPool))
    , _executor(_completionQueue, _threadPool) // now owns the completion queue
{
	_rpc->getStateMachine().setStateChangedCallback(
	    std::bind(&OutgoingRPC::onRPCStateChanged, this, std::placeholders::_1));
	_executor.start(dedicatedThreads);
}

OutgoingRPC::~OutgoingRPC()
{
	dispose();
}

/**
 *	Blocks until the client is connected thanks to the condition variable.
 */
bool OutgoingRPC::start()
{
	if (!_rpc->initialize()) return false;

	std::string serverAddress = _serverIp + ":" + std::to_string(_serverPort);

	auto channel = grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials());
	_stub = ghost::protobuf::connectiongrpc::ServerClientService::NewStub(channel);

	// Connect and wait that the connection succeeds
	RPCConnect<ReaderWriter, ContextType> connectOperation(_rpc, _stub, _completionQueue);
	connectOperation.start();
	while (connectOperation.isRunning()) _threadPool->yield(std::chrono::milliseconds(1));

	// If the connection failed, the RPC is not in state EXECUTING
	if (_rpc->getStateMachine().getState() != RPCStateMachine::EXECUTING) return false;

	// Start the reader
	startReader();
	startWriter();

	return true;
}

bool OutgoingRPC::stop()
{
	if (!_rpc->dispose()) return false;

	std::shared_ptr<RPCFinish<ReaderWriter, ContextType>> finishOperation;
	if (_rpc->getStateMachine().getState() == RPCStateMachine::DISPOSING)
	{
		finishOperation = std::make_shared<RPCFinish<ReaderWriter, ContextType>>(_rpc);
		finishOperation->start();
	}

	dispose();

	// finishOperation completed since _rpc->awaitFinished() returned, it is therefore okay to be deleted.
	return finishOperation->getStatus().ok() ||
	       finishOperation->getStatus().error_code() == grpc::StatusCode::CANCELLED;
}

bool OutgoingRPC::isRunning() const
{
	return _rpc->getStateMachine().getState() == RPCStateMachine::INITIALIZING ||
	       _rpc->getStateMachine().getState() == RPCStateMachine::EXECUTING ||
	       _rpc->getStateMachine().getState() == RPCStateMachine::INACTIVE;
}

void OutgoingRPC::setWriterSink(const std::shared_ptr<ghost::WriterSink>& sink)
{
	initWriter(_rpc, sink);
}

void OutgoingRPC::setReaderSink(const std::shared_ptr<ghost::ReaderSink>& sink)
{
	initReader(_rpc, sink);
}

void OutgoingRPC::onRPCStateChanged(RPCStateMachine::State newState)
{
	if (newState == RPCStateMachine::INACTIVE || newState == RPCStateMachine::FINISHED)
	{
		drainReader();
		drainWriter();
	}
}

void OutgoingRPC::dispose()
{
	stopReader();
	stopWriter();

	_rpc->awaitFinished();
	_executor.stop();
	_rpc->disposeGRPC();
}
