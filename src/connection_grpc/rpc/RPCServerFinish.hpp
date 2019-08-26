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

#ifndef GHOST_INTERNAL_NETWORK_RPCSERVERFINISH_HPP
#define GHOST_INTERNAL_NETWORK_RPCSERVERFINISH_HPP

#include <memory>
#include "RPCOperation.hpp"

namespace ghost
{
	namespace internal
	{
		template<typename ReaderWriter, typename ContextType>
		class RPCServerFinish : public RPCOperation<ReaderWriter, ContextType>
		{
		public:
			RPCServerFinish(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent, const grpc::Status& status);

		protected:
			bool initiateOperation() override;
			void onOperationSucceeded(bool rpcFinished) override;
			void onOperationFailed(bool rpcFinished) override;

		private:
			const grpc::Status& _status;
		};

		/////////////////////////// Template definition ///////////////////////////

		template<typename ReaderWriter, typename ContextType>
		RPCServerFinish<ReaderWriter, ContextType>::RPCServerFinish(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
			const grpc::Status& status)
			: RPCOperation(parent, false, false) // restart = false, blocking = false
			, _status(status)
		{
		}

		template<typename ReaderWriter, typename ContextType>
		bool RPCServerFinish<ReaderWriter, ContextType>::initiateOperation()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return false;

			rpc->getClient()->Finish(_status, &_operationCompletedCallback);
			return true;
		}

		template<typename ReaderWriter, typename ContextType>
		void RPCServerFinish<ReaderWriter, ContextType>::onOperationSucceeded(bool rpcFinished)
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::FINISHED);
		}

		template<typename ReaderWriter, typename ContextType>
		void RPCServerFinish<ReaderWriter, ContextType>::onOperationFailed(bool rpcFinished)
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::FINISHED);
		}

	}
}

#endif // GHOST_INTERNAL_NETWORK_RPCSERVERFINISH_HPP
