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

#include "RPCStateMachine.hpp"

using namespace ghost::internal;

RPCStateMachine::RPCStateMachine() : _state(CREATED)
{
}

RPCStateMachine::State RPCStateMachine::getState(bool lockk) const
{
	if (lockk)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _state;
	}

	return _state;
}

void RPCStateMachine::setState(State state, bool lockk)
{
	if (lockk) _mutex.lock();

	bool transitionAllowed = false;
	switch (_state)
	{
		case CREATED:
			transitionAllowed = (state != EXECUTING && state != INACTIVE);
			break;
		case INITIALIZING:
			transitionAllowed = (state != CREATED);
			break;
		case EXECUTING:
		case INACTIVE:
			transitionAllowed = (state == INACTIVE || state == DISPOSING || state == FINISHED);
			break;
		case DISPOSING:
			transitionAllowed = (state == FINISHED);
			break;
		case FINISHED:
			transitionAllowed = false;
			break;
		default:
			break;
	}

	if (transitionAllowed) _state = state;

	if (lockk) _mutex.unlock();

	// Call the callback if one was set.
	if (transitionAllowed && _stateChangedCallback) _stateChangedCallback(state);
}

void RPCStateMachine::setStateChangedCallback(const std::function<void(State)>& callback)
{
	_stateChangedCallback = callback;
}

std::unique_lock<std::mutex> RPCStateMachine::lock()
{
	return std::unique_lock<std::mutex>(_mutex);
}
