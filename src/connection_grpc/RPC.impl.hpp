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
RPC<ReaderWriter, ContextType>::RPC()
	:  _operationsRunning(0)
	, _context(new ContextType())
{
}

template<typename ReaderWriter, typename ContextType>
RPC<ReaderWriter, ContextType>::~RPC()
{
	disposeGRPC();
}

template<typename ReaderWriter, typename ContextType>
void RPC<ReaderWriter, ContextType>::disposeGRPC()
{
	_client.reset();
	_context.reset();
}

template<typename ReaderWriter, typename ContextType>
bool RPC<ReaderWriter, ContextType>::initialize()
{
	auto lock = _statemachine.lock();
	if (_statemachine.getState(false) == RPCStateMachine::CREATED)
	{
		_statemachine.setState(RPCStateMachine::INITIALIZING, false);
		return true;
	}

	return false;
}

template<typename ReaderWriter, typename ContextType>
bool RPC<ReaderWriter, ContextType>::dispose()
{
	auto lock = _statemachine.lock(); // mutex taken because user can call it concurrently - avoids entering twice in the if
	if (_statemachine.getState(false) == RPCStateMachine::EXECUTING || _statemachine.getState(false) == RPCStateMachine::INACTIVE)
	{
		_statemachine.setState(RPCStateMachine::DISPOSING, false);
		return true;
	}
	return false;
}

template<typename ReaderWriter, typename ContextType>
void RPC<ReaderWriter, ContextType>::startOperation()
{
	_operationsRunning++;
}

template<typename ReaderWriter, typename ContextType>
bool RPC<ReaderWriter, ContextType>::finishOperation()
{
	_operationsRunning--;

	return !(_operationsRunning == 0 && _statemachine.getState() == RPCStateMachine::FINISHED);
}

template<typename ReaderWriter, typename ContextType>
bool RPC<ReaderWriter, ContextType>::isFinished() const
{
	return _statemachine.getState() == RPCStateMachine::FINISHED && _operationsRunning == 0;
}

template<typename ReaderWriter, typename ContextType>
void RPC<ReaderWriter, ContextType>::awaitFinished()
{
	while (_statemachine.getState() != RPCStateMachine::FINISHED || _operationsRunning > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

template<typename ReaderWriter, typename ContextType>
const RPCStateMachine& RPC<ReaderWriter, ContextType>::getStateMachine() const
{
	return _statemachine;
}

template<typename ReaderWriter, typename ContextType>
RPCStateMachine& RPC<ReaderWriter, ContextType>::getStateMachine()
{
	return _statemachine;
}

template<typename ReaderWriter, typename ContextType>
void RPC<ReaderWriter, ContextType>::setClient(std::unique_ptr<ReaderWriter>&& client)
{
	_client = std::move(client);
}

template<typename ReaderWriter, typename ContextType>
const std::unique_ptr<ReaderWriter>& RPC<ReaderWriter, ContextType>::getClient() const
{
	return _client;
}

template<typename ReaderWriter, typename ContextType>
const std::unique_ptr<ContextType>& RPC<ReaderWriter, ContextType>::getContext() const
{
	return _context;
}
