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
RPCOperation<ReaderWriter, ContextType>::RPCOperation(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent, bool autoRestart, bool blocking)
	: _rpc(parent)
	, _autoRestart(autoRestart)
	, _blocking(blocking)
	, _state(OperationProgress::IDLE)
{
	_operationCompletedCallback = std::bind(&RPCOperation<ReaderWriter, ContextType>::onOperationCompleted, this, std::placeholders::_1);
}

template<typename ReaderWriter, typename ContextType>
RPCOperation<ReaderWriter, ContextType>::~RPCOperation()
{
	if (_executor.joinable())
		_executor.join();
}

template<typename ReaderWriter, typename ContextType>
bool RPCOperation<ReaderWriter, ContextType>::startAsync()
{
	_executor = std::thread([this] { start(); });
	return true;
}

template<typename ReaderWriter, typename ContextType>
bool RPCOperation<ReaderWriter, ContextType>::start()
{
	auto rpc = _rpc.lock();
	if (!rpc)
		return false;

	while (rpc->getStateMachine().getState() == RPCStateMachine::EXECUTING)
	{
		std::unique_lock<std::mutex> lock(_operationMutex);

		// Do not start operations parallely.
		if (_state == OperationProgress::IN_PROGRESS)
			return false;

		// Tell the implementation to do the call. It should return true if it actually made one.
		bool initiated = initiateOperation();
		if (initiated)
		{
			// Switch states to prevent parallel executions and tell the RPC that something is ongoing.
			_state = OperationProgress::IN_PROGRESS;
			rpc->startOperation();

			// If this call is blocking, wait until it finishes
			if (_blocking)
				_operationCompletedConditionVariable.wait(lock, [this] { return _state == OperationProgress::IDLE; });

			// If this call is not blocking, potential restarts will happen when the operation completes
			if (!_blocking || !_autoRestart)
				return true;
		}
	}
	return false;
}

template<typename ReaderWriter, typename ContextType>
void RPCOperation<ReaderWriter, ContextType>::onOperationCompleted(bool ok)
{
	auto rpc = _rpc.lock();
	if (!rpc)
		return;

	{
		std::unique_lock<std::mutex> lock(_operationMutex);

		// The operation completed, we can start another one right away.
		_state = OperationProgress::IDLE;

		// If the RPC should continue (next line returns true), continue.
		if (!rpc->finishOperation())
		{
			// Let the implementation do something with the result.
			if (ok)
				onOperationSucceeded();
			else
				onOperationFailed();
		}
	}

	// Free "start" threads that are potentially waiting for this.
	_operationCompletedConditionVariable.notify_one();

	// If autorestart is configured and the call is not blocking, do it here
	if (_autoRestart && !_blocking && ok && rpc->getStateMachine().getState() == RPCStateMachine::EXECUTING)
		start();
}
