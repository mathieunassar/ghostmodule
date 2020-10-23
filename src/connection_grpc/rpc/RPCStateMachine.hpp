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

#ifndef GHOST_INTERNAL_NETWORK_RPCSTATEMACHINE_HPP
#define GHOST_INTERNAL_NETWORK_RPCSTATEMACHINE_HPP

#include <functional>
#include <memory>
#include <mutex>

namespace ghost
{
namespace internal
{
/**
 *	State machine representing all the possible states of an RPC.
 */
class RPCStateMachine
{
public:
	enum State
	{
		/// The RPC is created, it was not initialized yet
		CREATED,
		/// The RPC has been initiated, and is connecting or waiting for an incoming connection.
		INITIALIZING,
		/// The RPC is connected to a peer and active.
		EXECUTING,
		/// Final: an operation failed, or the connection is shutting down
		INACTIVE,
		/// The RPC is being gracefully shut down by the user
		DISPOSING,
		/// Final: the RPC gracefully shut down and is now inactive
		FINISHED
	};

	RPCStateMachine();
	State getState(bool lock = true) const;
	void setState(State state, bool lock = true);
	void setStateChangedCallback(const std::function<void(State)>& callback);

	std::unique_lock<std::mutex> lock();

private:
	State _state;
	std::function<void(State)> _stateChangedCallback;
	mutable std::mutex _mutex;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_RPCSTATEMACHINE_HPP
