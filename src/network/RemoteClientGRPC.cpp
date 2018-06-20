#include "../../include/internal/network/RemoteClientGRPC.hpp"

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "../../include/internal/GenericMessageConverter.hpp"

using namespace ghost::internal;

RemoteClientGRPC::RemoteClientGRPC(protobuf::ServerClientService::AsyncService* service, grpc::ServerCompletionQueue* completionQueue,
		std::shared_ptr<ghost::ClientHandler> callback)
	: _operationsRunning(0)
	, _writeOperationInProgress(false)
	, _service(service)
	, _completionQueue(completionQueue)
	, _clientHandler(callback)
	, _client(&_context)
	, _status(INIT)
{
	_startedProcessor = std::bind(&RemoteClientGRPC::onStarted, this, std::placeholders::_1);
	_finishProcessor = std::bind(&RemoteClientGRPC::onFinished, this, std::placeholders::_1);
	_writtenProcessor = std::bind(&RemoteClientGRPC::onWriteFinished, this, std::placeholders::_1);
	_readProcessor = std::bind(&RemoteClientGRPC::onReadFinished, this, std::placeholders::_1);
	_doneProcessor = std::bind(&RemoteClientGRPC::onDone, this, std::placeholders::_1);

	_context.AsyncNotifyWhenDone(&_doneProcessor);

	start();
}

void RemoteClientGRPC::onStarted(bool ok)
{
	// restart the process of creating the request for the next client
	new RemoteClientGRPC(_service, _completionQueue, _clientHandler);

	if (!finishOperation())
		return;

	if (ok)
	{
		startOperation();
		// start reading some stuff
		_client.Read(&_incomingMessage, &_readProcessor);
	}
	else
		_status = FINISHED; // RPC could not start, finish it!

	execute();
}

void RemoteClientGRPC::onFinished(bool ok) // OK ignored, the RPC is going to be deleted anyway
{
	finishOperation(); // don't care about the result, this method does not start more RPC operation

	std::lock_guard<std::mutex> lock(_statusMutex);
	_status = FINISHED;
}

void RemoteClientGRPC::onWriteFinished(bool ok)
{
	if (!finishOperation())
		return;

	{ // remove the item that has just been sent
		std::lock_guard<std::mutex> lock(_writeQueueMutex);
		_writeQueue.pop_front();
	}
	_writeOperationInProgress = false;
	if (ok)
	{
		processWriteQueue();
	}
	else
	{
		std::lock_guard<std::mutex> lock(_statusMutex);
		_status = FINISHED;
	}
}

void RemoteClientGRPC::onReadFinished(bool ok)
{
	if (!finishOperation())
		return;

	if (ok)
	{
		std::lock_guard<std::mutex> lock(_readQueueMutex);
		_readQueue.push_back(_incomingMessage); // TODO maybe, client callback instead of queue?

		startOperation();
		_client.Read(&_incomingMessage, &_readProcessor);
	}
	else
	{
		std::lock_guard<std::mutex> lock(_statusMutex);
		_status = FINISHED;
	}
}

void RemoteClientGRPC::onDone(bool ok)
{
	std::lock_guard<std::mutex> lock(_statusMutex);
	_status = FINISHED;
}

void RemoteClientGRPC::startOperation()
{
	_operationsRunning++;
}

bool RemoteClientGRPC::finishOperation()
{
	_operationsRunning--;

	return !(_operationsRunning == 0 && getStatus() == FINISHED);
}

void RemoteClientGRPC::execute()
{
	if (_status == EXECUTING) // if the RPC failed during onStarted, its state is here FINISHED
	{
		// call the application code
		bool continueExecution = true;
		if (_clientHandler)
			continueExecution = _clientHandler->handle(*this);

		stop();
	}

	while (getStatus() != FINISHED || _operationsRunning > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	delete this;
}

bool RemoteClientGRPC::start()
{
	if (_status == INIT) // No mutex is taken for _status here because at this point, the object is only monitored by its constructor
	{
		_status = EXECUTING;

		startOperation();

		// create the request, this will be available soon in the completion queue, which will call "proceedProcessor"
		_service->Requestconnect(&_context, &_client, _completionQueue, _completionQueue, &_startedProcessor);

		return true;
	}
	return false;
}

bool RemoteClientGRPC::stop()
{
	std::lock_guard<std::mutex> lock(_statusMutex); // mutex taken because user can call it concurrently - avoids entering twice in the if
	if (_status == EXECUTING)
	{
		_status = DISPOSING;

		startOperation();
		_client.Finish(grpc::Status::OK, &_finishProcessor);
		
		return true;
	}
	return false;
}

bool RemoteClientGRPC::isRunning() const
{
	std::lock_guard<std::mutex> lock(_statusMutex);
	return _status == EXECUTING;
}

bool RemoteClientGRPC::receive(ghost::Message& message)
{
	if (getStatus() != EXECUTING)
	{
		return false; // Rpc is finishing or had an error
	}

	_readQueueMutex.lock();
	size_t queueSize = _readQueue.size();
	_readQueueMutex.unlock();

	// TODO non blocking calls in connection configuration
	while (queueSize == 0) // blocks until a message is there, use blocking queue instead!
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		_readQueueMutex.lock();
		queueSize = _readQueue.size();
		_readQueueMutex.unlock();
	}

	std::lock_guard<std::mutex> lock(_readQueueMutex);
	_nextMessage = _readQueue.front();
	_readQueue.pop_front();

	return GenericMessageConverter::parse(_nextMessage, message);
}

bool RemoteClientGRPC::lastReceived(ghost::Message& message)
{
	return GenericMessageConverter::parse(_nextMessage, message);
}

bool RemoteClientGRPC::send(const ghost::Message& message)
{
	if (getStatus() != EXECUTING)
	{
		return false; // Rpc is finishing or had an error
	}

	google::protobuf::Any any;
	bool createSuccess = GenericMessageConverter::create(any, message);
	if (!createSuccess)
	{
		return false; // conversion failed, i.e. there is no protobuf message in the input or the serialization failed (in case of a user format)
	}

	{ // add the message in the write queue
		std::lock_guard<std::mutex> lock(_writeQueueMutex);
		_writeQueue.push_back(any);
	}

	processWriteQueue();

	_writeQueueMutex.lock();
	size_t queueSize = _writeQueue.size();
	_writeQueueMutex.unlock();

	// TODO unblocking calls in the connection configuration
	while (queueSize != 0) // blocks until the message is gone, use blocking queue instead!
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		_writeQueueMutex.lock();
		queueSize = _writeQueue.size();
		_writeQueueMutex.unlock();
	}

	return true;
}

void RemoteClientGRPC::processWriteQueue()
{
	std::lock_guard<std::mutex> lock(_writeQueueMutex);
	if (!_writeOperationInProgress && !_writeQueue.empty())
	{
		startOperation();
		_writeOperationInProgress = true;
		_client.Write(_writeQueue.front(), &_writtenProcessor);
	}
}

RemoteClientGRPC::CallStatus RemoteClientGRPC::getStatus() const
{
	std::lock_guard<std::mutex> lock(_statusMutex);
	return _status;
}
