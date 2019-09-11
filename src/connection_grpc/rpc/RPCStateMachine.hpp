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

#ifndef GHOST_INTERNAL_NETWORK_RPCSTATEMACHINE_HPP
#define GHOST_INTERNAL_NETWORK_RPCSTATEMACHINE_HPP

#include <mutex>
#include <memory>
#include <functional>

namespace ghost
{
	namespace internal
	{
		class RPCStateMachine
		{
		public:
			enum State
			{
				CREATED,
				INITIALIZING,
				EXECUTING, INACTIVE,
				DISPOSING,
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
	}
}

#endif // GHOST_INTERNAL_NETWORK_RPCSTATEMACHINE_HPP
