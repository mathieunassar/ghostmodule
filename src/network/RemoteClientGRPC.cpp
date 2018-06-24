#include "../../include/internal/network/RemoteClientGRPC.hpp"

using namespace ghost::internal;

RemoteClientGRPC::RemoteClientGRPC(const ghost::NetworkConnectionConfiguration& config,
		protobuf::ServerClientService::AsyncService* service, grpc::ServerCompletionQueue* completionQueue,
		std::shared_ptr<ghost::ClientHandler> callback)
	: BaseClientGRPC(config, completionQueue)
	, _service(service)
	, _clientHandler(callback)
{
	_client = std::unique_ptr<grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>>
		(new grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>(_context.get()));

	_startedProcessor = std::bind(&RemoteClientGRPC::onStarted, this, std::placeholders::_1);
	_finishProcessor = std::bind(&RemoteClientGRPC::onFinished, this, std::placeholders::_1);
	_doneProcessor = std::bind(&RemoteClientGRPC::onDone, this, std::placeholders::_1);

	_context->AsyncNotifyWhenDone(&_doneProcessor);

	start();
}

bool RemoteClientGRPC::start()
{
	if (!BaseClientGRPC::start())
		return false;

	startOperation();
	// create the request, this will be available soon in the completion queue, which will call "proceedProcessor"
	auto cq = static_cast<grpc::ServerCompletionQueue*>(_completionQueue);
	_service->Requestconnect(_context.get(), _client.get(), _completionQueue, cq, &_startedProcessor);

	return true;
}

void RemoteClientGRPC::onStarted(bool ok)
{
	if (ok) // if not ok for this call, then the completion queue has been shut down
	{
		// restart the process of creating the request for the next client
		auto cq = static_cast<grpc::ServerCompletionQueue*>(_completionQueue);
		new RemoteClientGRPC(_configuration, _service, cq, _clientHandler);
	}

	if (!finishOperation())
		return;

	if (ok)
		startReader();
	else
		_statemachine.setState(RPCStateMachine::FINISHED); // RPC could not start, finish it!

	execute();
}

void RemoteClientGRPC::onFinished(bool ok) // OK ignored, the RPC is going to be deleted anyway
{
	finishOperation(); // don't care about the result, this method does not start more RPC operation

	_statemachine.setState(RPCStateMachine::FINISHED);
}

bool RemoteClientGRPC::stop()
{
	if (!BaseClientGRPC::stop())
		return false;

	startOperation();
	_client->Finish(grpc::Status::OK, &_finishProcessor);
	return true;
}

void RemoteClientGRPC::onDone(bool ok)
{
	_statemachine.setState(RPCStateMachine::FINISHED);
}

void RemoteClientGRPC::execute()
{
	if (_statemachine.getState() == RPCStateMachine::EXECUTING) // if the RPC failed during onStarted, its state is here FINISHED
	{
		// call the application code
		bool continueExecution = true;
		if (_clientHandler)
			continueExecution = _clientHandler->handle(*this);

		stop();
	}

	// only delete this object when the state is finished and no other operations are running
	// if operations were running after this gets deleted, function pointers to callbacks would be lead to segmentation faults.
	awaitFinished();

	disposeGRPC(); // this method exists because it seems that gRPC needs a specific destruction order that the default destructor does not guarantee.
	delete this;
}
