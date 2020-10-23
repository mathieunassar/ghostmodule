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

template <typename ReaderWriter, typename ContextType>
RPCOperation<ReaderWriter, ContextType>::RPCOperation(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent)
    : _rpc(parent), _state(OperationProgress::IDLE)
{
	_operationCompletedCallback =
	    std::bind(&RPCOperation<ReaderWriter, ContextType>::onOperationCompleted, this, std::placeholders::_1);
}

template <typename ReaderWriter, typename ContextType>
bool RPCOperation<ReaderWriter, ContextType>::start()
{
	auto rpc = _rpc.lock();
	if (!rpc) return false;

	// Do not start RPCs that are already finished
	auto rpcState = rpc->getStateMachine().getState();
	if (rpcState == RPCStateMachine::FINISHED || rpcState == RPCStateMachine::INACTIVE) return false;

	std::unique_lock<std::mutex> lock(_operationMutex);

	// Do not start RPCs that are already started
	if (_state == OperationProgress::IN_PROGRESS) return false;

	// Start the implementation
	bool initiated = initiateOperation();
	if (!initiated) return false;

	// If it worked, switch the operation progress state and record that an operation is running
	_state = OperationProgress::IN_PROGRESS;
	if (accountsAsRunningOperation()) rpc->startOperation();

	return true;
}

template <typename ReaderWriter, typename ContextType>
bool RPCOperation<ReaderWriter, ContextType>::isRunning() const
{
	std::unique_lock<std::mutex> lock(_operationMutex);
	return _state == OperationProgress::IN_PROGRESS;
}

template <typename ReaderWriter, typename ContextType>
void RPCOperation<ReaderWriter, ContextType>::onFinish(const std::function<void()>& callback)
{
	_finishCallback = callback;
}

template <typename ReaderWriter, typename ContextType>
bool RPCOperation<ReaderWriter, ContextType>::accountsAsRunningOperation() const
{
	return true;
}

template <typename ReaderWriter, typename ContextType>
void RPCOperation<ReaderWriter, ContextType>::onOperationCompleted(bool ok)
{
	auto rpc = _rpc.lock();
	if (!rpc) return;

	std::unique_lock<std::mutex> lock(_operationMutex);

	// The operation completed, we can start another one right away.
	_state = OperationProgress::IDLE;

	// tell the RPC that an operation finished
	if (accountsAsRunningOperation()) rpc->finishOperation();

	// Let the implementation do something with the result.
	if (ok)
		onOperationSucceeded();
	else
		onOperationFailed();

	// free the mutex because the callback may trigger the destruction of this object
	lock.unlock();
	if (_finishCallback) _finishCallback();
}
