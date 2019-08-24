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

#ifndef GHOST_INTERNAL_NETWORK_RPCOPERATION_HPP
#define GHOST_INTERNAL_NETWORK_RPCOPERATION_HPP

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "RPC.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		 *	Base class for operations on a gRPC async connection (represented by ghost::internal::RPC).
		 *	This class provides a base implementation to handle a completed completion queue tag, and
		 *	manages the lifetime of the operation.
		 *	The class ensures that the operation is not executed twice simultaneously.
		 *
		 *	Operations can be configured to be blocking and/or automatically restarted.
		 *	- If an operation is non-blocking and autorestarted, then it will be restarted from the
		 *	completion queue thread (calling start will not block).
		 *	- If an operation is blocking and autorestarted, then calling start is blocking until the RPC
		 *	is finsihed.
		 */
		template<typename ReaderWriter, typename ContextType>
		class RPCOperation
		{
		public:
			enum class OperationProgress
			{
				IDLE,
				IN_PROGRESS
			};

			RPCOperation(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent, bool autoRestart, bool blocking);
			virtual ~RPCOperation();

			bool startAsync();
			bool start();

		protected:
			/// Push an operation in the RPC's completion queue.
			virtual bool initiateOperation() = 0;
			/// This will be called if the processor is called with ok = false
			virtual void onOperationSucceeded() {}
			/// This will be called if the processor is called with ok = true
			virtual void onOperationFailed() {}

			std::weak_ptr<RPC<ReaderWriter, ContextType>> _rpc;
			bool _autoRestart;
			bool _blocking;
			std::thread _executor;
			OperationProgress _state;
			std::mutex _operationMutex;
			std::condition_variable _operationCompletedConditionVariable;
			std::function<void(bool)> _operationCompletedCallback;

		private:
			void onOperationCompleted(bool ok);
		};

		#include "RPCOperation.impl.hpp"
	}
}

#endif // GHOST_INTERNAL_NETWORK_RPCOPERATION_HPP
