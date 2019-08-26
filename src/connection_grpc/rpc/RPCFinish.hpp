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

#ifndef GHOST_INTERNAL_NETWORK_RPCFINISH_HPP
#define GHOST_INTERNAL_NETWORK_RPCFINISH_HPP

#include <memory>

#include "RPCOperation.hpp"

namespace ghost
{
	namespace internal
	{
		template<typename ReaderWriter, typename ContextType>
		class RPCFinish : public RPCOperation<ReaderWriter, ContextType>
		{
		public:
			RPCFinish(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent, const grpc::Status& status = grpc::Status::CANCELLED);

			const grpc::Status& getStatus() const;

		protected:
			bool initiateOperation() override;
			void onOperationSucceeded(bool rpcFinished) override;
			void onOperationFailed(bool rpcFinished) override;

		private:
			grpc::Status _status;
		};

		/////////////////////////// Template definition ///////////////////////////

		template<typename ReaderWriter, typename ContextType>
		RPCFinish<ReaderWriter, ContextType>::RPCFinish(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
			const grpc::Status& status)
			: RPCOperation(parent, false, true) // restart = false, blocking = false
			, _status(status)
		{
		}

		template<typename ReaderWriter, typename ContextType>
		bool RPCFinish<ReaderWriter, ContextType>::initiateOperation()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return false;

			rpc->getContext()->TryCancel();
			rpc->getClient()->Finish(&_status, &_operationCompletedCallback);
			return true;
		}

		template<typename ReaderWriter, typename ContextType>
		void RPCFinish<ReaderWriter, ContextType>::onOperationSucceeded(bool rpcFinished)
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::FINISHED);
		}

		template<typename ReaderWriter, typename ContextType>
		void RPCFinish<ReaderWriter, ContextType>::onOperationFailed(bool rpcFinished)
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::FINISHED);
		}
		
		template<typename ReaderWriter, typename ContextType>
		const grpc::Status& RPCFinish<ReaderWriter, ContextType>::getStatus() const
		{
			return _status;
		}
	}
}

#endif // GHOST_INTERNAL_NETWORK_RPCCONNECT_HPP
