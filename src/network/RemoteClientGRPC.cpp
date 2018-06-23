#include "../../include/internal/network/RemoteClientGRPC.hpp"

using namespace ghost::internal;

RemoteClientGRPC::RemoteClientGRPC(protobuf::ServerClientService::AsyncService* service, grpc::ServerCompletionQueue* completionQueue,
		std::shared_ptr<ghost::ClientHandler> callback)
	: BaseClientGRPC(completionQueue)
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

RemoteClientGRPC::~RemoteClientGRPC()
{
	std::cout << "destructor of remote client called: " << _operationsRunning << std::endl;
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
	// restart the process of creating the request for the next client
	auto cq = static_cast<grpc::ServerCompletionQueue*>(_completionQueue);
	new RemoteClientGRPC(_service, cq, _clientHandler);

	if (!finishOperation())
		return;

	if (ok)
		startReader();
	else
		setStatus(FINISHED); // RPC could not start, finish it!

	execute();
}

void RemoteClientGRPC::onFinished(bool ok) // OK ignored, the RPC is going to be deleted anyway
{
	finishOperation(); // don't care about the result, this method does not start more RPC operation

	setStatus(FINISHED);
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
	setStatus(FINISHED);
}

void RemoteClientGRPC::execute()
{
	if (getStatus() == EXECUTING) // if the RPC failed during onStarted, its state is here FINISHED
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

	disposeGRPC();
	delete this;
}
