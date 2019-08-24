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

#ifndef GHOST_INTERNAL_NETWORK_RPCDONE_HPP
#define GHOST_INTERNAL_NETWORK_RPCDONE_HPP

#include <memory>

#include "RPCOperation.hpp"

namespace ghost
{
	namespace internal
	{
		template<typename ReaderWriter, typename ContextType>
		class RPCDone : public RPCOperation<ReaderWriter, ContextType>
		{
		public:
			RPCDone(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent);

		protected:
			bool initiateOperation() override;
			void onOperationSucceeded() override;
			void onOperationFailed() override;
		};

		/////////////////////////// Template definition ///////////////////////////

		template<typename ReaderWriter, typename ContextType>
		RPCDone<ReaderWriter, ContextType>::RPCDone(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent)
			: RPCOperation(parent, false, false) // restart = false, blocking = false
		{
		}

		template<typename ReaderWriter, typename ContextType>
		bool RPCDone<ReaderWriter, ContextType>::initiateOperation()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return false;

			rpc->getContext()->AsyncNotifyWhenDone(&_operationCompletedCallback);
			return true;
		}

		template<typename ReaderWriter, typename ContextType>
		void RPCDone<ReaderWriter, ContextType>::onOperationSucceeded()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::FINISHED);
		}

		template<typename ReaderWriter, typename ContextType>
		void RPCDone<ReaderWriter, ContextType>::onOperationFailed()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::FINISHED);
		}

	}
}

#endif // GHOST_INTERNAL_NETWORK_RPCDONE_HPP
