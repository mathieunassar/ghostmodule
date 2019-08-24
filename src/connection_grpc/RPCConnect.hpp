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

#ifndef GHOST_INTERNAL_NETWORK_RPCCONNECT_HPP
#define GHOST_INTERNAL_NETWORK_RPCCONNECT_HPP

#include "RPCOperation.hpp"

namespace ghost
{
	namespace internal
	{
		template<typename ReaderWriter, typename ContextType>
		class RPCConnect : public RPCOperation<ReaderWriter, ContextType>
		{
		public:
			RPCConnect(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
				const std::shared_ptr<ghost::protobuf::connectiongrpc::ServerClientService::Stub>& stub,
				grpc::CompletionQueue* completionQueue);

		protected:
			bool initiateOperation() override;
			void onOperationSucceeded() override;
			void onOperationFailed() override;

		private:
			std::shared_ptr<ghost::protobuf::connectiongrpc::ServerClientService::Stub> _stub;
			grpc::CompletionQueue* _completionQueue;
		};

		/////////////////////////// Template definition ///////////////////////////

		template<typename ReaderWriter, typename ContextType>
		RPCConnect<ReaderWriter, ContextType>::RPCConnect(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
			const std::shared_ptr<ghost::protobuf::connectiongrpc::ServerClientService::Stub>& stub,
			grpc::CompletionQueue* completionQueue)
			: RPCOperation(parent, false, true) // restart = false, blocking = true
			, _stub(stub)
			, _completionQueue(completionQueue)
		{
		}

		template<typename ReaderWriter, typename ContextType>
		bool RPCConnect<ReaderWriter, ContextType>::initiateOperation()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return false;

			rpc->setClient(_stub->Asyncconnect(rpc->getContext().get(), _completionQueue, &_operationCompletedCallback));
			return true;
		}

		template<typename ReaderWriter, typename ContextType>
		void RPCConnect<ReaderWriter, ContextType>::onOperationSucceeded()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::EXECUTING);
		}

		template<typename ReaderWriter, typename ContextType>
		void RPCConnect<ReaderWriter, ContextType>::onOperationFailed()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::FINISHED); // RPC could not start, finish it!
		}

	}
}

#endif // GHOST_INTERNAL_NETWORK_RPCCONNECT_HPP
