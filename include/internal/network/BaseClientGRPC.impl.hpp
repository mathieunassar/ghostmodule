template<typename ReaderWriter, typename ContextType>
BaseClientGRPC<ReaderWriter, ContextType>::BaseClientGRPC(grpc::CompletionQueue* completionQueue)
	: _operationsRunning(0)
	, _writeOperationInProgress(false)
	, _completionQueue(completionQueue)
	, _context(new ContextType())
	, _status(INIT)
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
	{
		processWriteQueue();
	}
	else
	{
		setStatus(INACTIVE);
	}
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
	{
		std::cout << "read not ok, setting to finished (current state: )" << getStatus() << std::endl;
		setStatus(INACTIVE);
	}
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

	return !(_operationsRunning == 0 && getStatus() == FINISHED);
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::awaitFinished()
{
	while (getStatus() != FINISHED || _operationsRunning > 0)
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
	std::lock_guard<std::mutex> lock(_statusMutex);
	if (_status == INIT) // No mutex is taken for _status here because at this point, the object is only monitored by its constructor
	{
		_status = EXECUTING;

		return true;
	}
	return false;
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::stop()
{
	std::lock_guard<std::mutex> lock(_statusMutex); // mutex taken because user can call it concurrently - avoids entering twice in the if
	if (_status == EXECUTING || _status == INACTIVE)
	{
		_status = DISPOSING;
		
		return true;
	}
	return false;
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::isRunning() const
{
	return getStatus() == EXECUTING;
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::receive(ghost::Message& message)
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

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::lastReceived(ghost::Message& message)
{
	return GenericMessageConverter::parse(_nextMessage, message);
}

template<typename ReaderWriter, typename ContextType>
bool BaseClientGRPC<ReaderWriter, ContextType>::send(const ghost::Message& message)
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

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::setStatus(CallStatus status)
{
	std::lock_guard<std::mutex> lock(_statusMutex);
	bool transitionAllowed = false;
	switch (_status)
	{
	case INIT:
		transitionAllowed = true;
		break;
	case EXECUTING:
	case INACTIVE:
		transitionAllowed = (status == DISPOSING || status == FINISHED);
		break;
	case DISPOSING:
		transitionAllowed = (status == FINISHED);
		break;
	case FINISHED:
		transitionAllowed = false;
		break;
	default: break;
	}

	if (transitionAllowed)
		_status = status;
}
