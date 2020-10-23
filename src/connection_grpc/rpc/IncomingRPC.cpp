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

#include "IncomingRPC.hpp"

#include "../RemoteClientGRPC.hpp"

using namespace ghost::internal;

IncomingRPC::IncomingRPC(ghost::protobuf::connectiongrpc::ServerClientService::AsyncService* service,
			 grpc::ServerCompletionQueue* completionQueue,
			 const std::shared_ptr<ghost::ThreadPool>& threadPool,
			 const std::function<void(std::shared_ptr<RemoteClientGRPC>)>& clientConnectedCallback)
    : WriterRPC(threadPool)
    , _serverCallback(clientConnectedCallback)
    , _threadPool(threadPool)
    , _rpc(std::make_shared<RPC<ReaderWriter, ContextType>>(threadPool))
    , _requestOperation(std::make_shared<RPCRequest<ReaderWriter, ContextType, ServiceType>>(
	  _rpc, service, completionQueue, completionQueue))
    , _doneOperation(std::make_shared<RPCDone<ReaderWriter, ContextType>>(_rpc))
{
	auto rpcCallback = std::bind(&IncomingRPC::onRPCConnected, this);
	_requestOperation->setConnectionCallback(rpcCallback);

	_rpc->setClient(std::make_unique<grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>>(
	    _rpc->getContext().get()));
	_rpc->getStateMachine().setStateChangedCallback(
	    std::bind(&IncomingRPC::onRPCStateChanged, this, std::placeholders::_1));

	initReader(_rpc);
	initWriter(_rpc);

	_doneOperation->start();
	start();
}

IncomingRPC::~IncomingRPC()
{
	dispose();
}

bool IncomingRPC::start()
{
	if (!_rpc->initialize()) return false;

	return _requestOperation->start();
}

bool IncomingRPC::stop(const grpc::Status& status)
{
	if (!_rpc->dispose()) return false;

	// if the state switched to DISPOSING, start the finish operation
	if (_rpc->getStateMachine().getState() == RPCStateMachine::DISPOSING)
	{
		_finishOperation = std::make_shared<RPCServerFinish<ReaderWriter, ContextType>>(_rpc, status);
		_finishOperation->start();
	}

	dispose();

	return true;
}

void IncomingRPC::dispose()
{
	stopReader();
	stopWriter();

	// only delete this object when the state is finished and no other operations are running
	// if operations were running after this gets deleted, function pointers to callbacks would be lead to
	// segmentation faults.
	_rpc->awaitFinished();

	_rpc->disposeGRPC(); // this method exists because it seems that gRPC needs a specific destruction order that
			     // the default destructor does not guarantee.
}

bool IncomingRPC::isFinished() const
{
	return _rpc->isFinished();
}

void IncomingRPC::setParent(std::weak_ptr<RemoteClientGRPC> parent)
{
	_parent = parent;
}

std::shared_ptr<RemoteClientGRPC> IncomingRPC::getParent()
{
	return _parent.lock();
}

void IncomingRPC::onRPCConnected()
{
	auto parent = _parent.lock();

	if (_serverCallback && parent)
		_serverCallback(parent);
	else
		stop();
}

void IncomingRPC::onRPCStateChanged(RPCStateMachine::State newState)
{
	if (newState == RPCStateMachine::INACTIVE || newState == RPCStateMachine::FINISHED)
	{
		drainReader();
		drainWriter();
	}
}
