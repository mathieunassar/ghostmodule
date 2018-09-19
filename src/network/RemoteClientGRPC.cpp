#include "../../include/internal/network/RemoteClientGRPC.hpp"

using namespace ghost::internal;

RemoteClientGRPC::RemoteClientGRPC(const ghost::NetworkConnectionConfiguration& config,
		protobuf::ServerClientService::AsyncService* service, grpc::ServerCompletionQueue* completionQueue,
		std::shared_ptr<ghost::ClientHandler> callback,
		ClientManager* clientManager,
		ghost::Server* server)
	: BaseClientGRPC(config, completionQueue)
	, _service(service)
	, _clientHandler(callback)
	, _clientManager(clientManager)
	, _server(server)
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
	if (_executionThread.joinable())
		_executionThread.join();
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
		auto client = std::make_shared<RemoteClientGRPC>(_configuration, _service, cq, _clientHandler, _clientManager, _server);
		_clientManager->addClient(client);
	}

	if (!finishOperation())
	{
		disposeGRPC();
		_clientManager->releaseClient(shared_from_this());
	}

	if (ok)
	{
		_statemachine.setState(RPCStateMachine::EXECUTING);
		startReader();
		if (isWriterConfigured())
			startWriter();
	}
	else
		_statemachine.setState(RPCStateMachine::FINISHED); // RPC could not start, finish it!

	_executionThread = std::thread(&RemoteClientGRPC::execute, this);
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

	// if the state switched to DISPOSING, start the finish operation
	if (_statemachine.getState() == RPCStateMachine::DISPOSING)
	{
		startOperation();
		_client->Finish(grpc::Status::OK, &_finishProcessor);
	}
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
		bool keepClientAlive = false;
		
		if (_clientHandler)
			continueExecution = _clientHandler->handle(shared_from_this(), keepClientAlive);

		// only stop the client if the user left "keepClientAlive" to false
		if (!keepClientAlive)
			stop();

		// if continueExecution is false, stop the server
		if (!continueExecution)
			_server->stop();
	}

	// only delete this object when the state is finished and no other operations are running
	// if operations were running after this gets deleted, function pointers to callbacks would be lead to segmentation faults.
	awaitFinished();

	disposeGRPC(); // this method exists because it seems that gRPC needs a specific destruction order that the default destructor does not guarantee.
	_clientManager->releaseClient(shared_from_this());
}
