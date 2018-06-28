template<typename ReaderWriter, typename ContextType>
BaseClientGRPC<ReaderWriter, ContextType>::BaseClientGRPC(const ghost::NetworkConnectionConfiguration& config, grpc::CompletionQueue* completionQueue)
	: _operationsRunning(0)
	, _writeOperationInProgress(false)
	, _configuration(config)
	, _completionQueue(completionQueue)
	, _context(new ContextType())
{
	_writtenProcessor = std::bind(&BaseClientGRPC<ReaderWriter, ContextType>::onWriteFinished, this, std::placeholders::_1);
	_readProcessor = std::bind(&BaseClientGRPC<ReaderWriter, ContextType>::onReadFinished, this, std::placeholders::_1);
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::onWriteFinished(bool ok)
{
	if (!finishOperation())
		return;

	{ // remove the item that has just been sent
		std::lock_guard<std::mutex> lock(_writeQueueMutex);
		_writeQueue.pop_front();
	}
	_writeOperationInProgress = false;

	if (ok)
		processWriteQueue();
	else
		_statemachine.setState(RPCStateMachine::INACTIVE);
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::onReadFinished(bool ok)
{
	if (!finishOperation())
		return;

	if (ok)
	{
		std::lock_guard<std::mutex> lock(_readQueueMutex);
		_readQueue.push_back(_incomingMessage); // TODO maybe, client callback instead of queue?

		startOperation();
		_client->Read(&_incomingMessage, &_readProcessor);
	}
	else
		_statemachine.setState(RPCStateMachine::INACTIVE);
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::startOperation()
{
	_operationsRunning++;
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::finishOperation()
{
	_operationsRunning--;

	return !(_operationsRunning == 0 && _statemachine.getState() == RPCStateMachine::FINISHED);
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::awaitFinished()
{
	while (_statemachine.getState() != RPCStateMachine::FINISHED || _operationsRunning > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::startReader()
{
	startOperation();
	// start reading some stuff
	_client->Read(&_incomingMessage, &_readProcessor);
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::start()
{
	_statemachine.lock();
	if (_statemachine.getState(false) == RPCStateMachine::CREATED)
	{
		_statemachine.setState(RPCStateMachine::INIT, false);

		_statemachine.unlock();
		return true;
	}
	_statemachine.unlock();
	return false;
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::stop()
{
	_statemachine.lock(); // mutex taken because user can call it concurrently - avoids entering twice in the if
	if (_statemachine.getState(false) == RPCStateMachine::EXECUTING || _statemachine.getState(false) == RPCStateMachine::INACTIVE)
	{
		_statemachine.setState(RPCStateMachine::DISPOSING, false);
		
		_statemachine.unlock();
		return true;
	}
	_statemachine.unlock();
	return false;
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::isRunning() const
{
	return _statemachine.getState() == RPCStateMachine::EXECUTING;
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::receive(ghost::Message& message)
{
	if (_statemachine.getState() != RPCStateMachine::EXECUTING)
	{
		return false; // Rpc is finishing or had an error
	}

	_readQueueMutex.lock();
	size_t queueSize = _readQueue.size();
	_readQueueMutex.unlock();

	if (!_configuration.isOperationBlocking() && queueSize == 0)
		return false; // call is non blocking and there is nothing to read, return false. User can find out that there is no issue by calling "isRunnung()"

	// else -> either connection is blocking or there is something to read

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

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::lastReceived(ghost::Message& message)
{
	return GenericMessageConverter::parse(_nextMessage, message);
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::send(const ghost::Message& message)
{
	if (_statemachine.getState() != RPCStateMachine::EXECUTING)
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

	if (!_configuration.isOperationBlocking())
		return true; // call is non-blocking, return directly

	// else -> call is blocking, we wait here until the message is sent

	_writeQueueMutex.lock();
	size_t queueSize = _writeQueue.size();
	_writeQueueMutex.unlock();

	while (queueSize != 0) // blocks until the message is gone, use blocking queue instead!
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		_writeQueueMutex.lock();
		queueSize = _writeQueue.size();
		_writeQueueMutex.unlock();
	}

	return true;
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::processWriteQueue()
{
	std::lock_guard<std::mutex> lock(_writeQueueMutex);
	if (!_writeOperationInProgress && !_writeQueue.empty())
	{
		startOperation();
		_writeOperationInProgress = true;
		_client->Write(_writeQueue.front(), &_writtenProcessor);
	}
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::disposeGRPC()
{
	_client.reset();
	_context.reset();
}
