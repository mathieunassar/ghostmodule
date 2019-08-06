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

template<typename ReaderWriter, typename ContextType>
BaseClientGRPC<ReaderWriter, ContextType>::BaseClientGRPC(const ghost::NetworkConnectionConfiguration& config, grpc::CompletionQueue* completionQueue)
	: Client(config)
	, _operationsRunning(0)
	, _writeInProgress(false)
	, _configuration(config)
	, _completionQueue(completionQueue)
	, _context(new ContextType())
{
	_writtenProcessor = std::bind(&BaseClientGRPC<ReaderWriter, ContextType>::onWriteFinished, this, std::placeholders::_1);
	_readProcessor = std::bind(&BaseClientGRPC<ReaderWriter, ContextType>::onReadFinished, this, std::placeholders::_1);
}

template<typename ReaderWriter, typename ContextType>
BaseClientGRPC<ReaderWriter, ContextType>::~BaseClientGRPC()
{
	if (_writerThread.joinable())
		_writerThread.join();

	disposeGRPC();
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::onWriteFinished(bool ok)
{
	{
		std::unique_lock<std::mutex> lk(_writerMutex);

		if (!finishOperation())
			return;

		getWriterSink()->pop();

		if (!ok)
		{
			_statemachine.setState(RPCStateMachine::INACTIVE);
		}

		_writeInProgress = false;
	}
	_writerConditionVariable.notify_one();
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::onReadFinished(bool ok)
{
	if (!finishOperation())
		return;

	if (ok)
	{
		getReaderSink()->put(_incomingMessage);

		startReader();
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
bool BaseClientGRPC<ReaderWriter, ContextType>::isFinished() const
{
	return _statemachine.getState() == RPCStateMachine::FINISHED && _operationsRunning == 0;
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
bool BaseClientGRPC<ReaderWriter, ContextType>::isWriterConfigured() const
{
	return !_configuration.hasAttribute(BASE_CLIENT_GRPC_CONFIG_NOWRITER);
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::startReader()
{
	startOperation();
	// start reading some stuff
	_client->Read(&_incomingMessage, &_readProcessor);
}

template<typename ReaderWriter, typename ContextType>
void BaseClientGRPC<ReaderWriter, ContextType>::startWriter()
{
	_writerThread = std::thread([this]
	{
		while (_statemachine.getState() == RPCStateMachine::EXECUTING)
		{
			google::protobuf::Any message;
			bool success = false;
			while (!success && _statemachine.getState() == RPCStateMachine::EXECUTING)
			{
				success = getWriterSink()->get(message, std::chrono::milliseconds(10));
			}

			if (success)
			{
				startOperation();

				std::unique_lock<std::mutex> lk(_writerMutex);
				
				_writeInProgress = true;
				_client->Write(message, &_writtenProcessor);

				_writerConditionVariable.wait(lk, [this] {return !_writeInProgress; });
			}
		}
	});
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
void BaseClientGRPC<ReaderWriter, ContextType>::disposeGRPC()
{
	_client.reset();
	_context.reset();
}
