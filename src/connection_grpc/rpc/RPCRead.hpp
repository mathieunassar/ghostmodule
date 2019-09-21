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

#ifndef GHOST_INTERNAL_NETWORK_RPCREAD_HPP
#define GHOST_INTERNAL_NETWORK_RPCREAD_HPP

#include <memory>

#include <ghost/connection/ReaderSink.hpp>

#include "RPCOperation.hpp"

namespace ghost
{
	namespace internal
	{
		template<typename ReaderWriter, typename ContextType, typename ReadMessageType>
		class RPCRead : public RPCOperation<ReaderWriter, ContextType>
		{
		public:
			RPCRead(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
				const std::shared_ptr<ghost::ReaderSink>& readerSink);
			~RPCRead();

		protected:
			bool initiateOperation() override;
			void onOperationSucceeded(bool rpcFinished) override;
			void onOperationFailed(bool rpcFinished) override;

		private:
			ReadMessageType _incomingMessage;
			std::shared_ptr<ghost::ReaderSink> _readerSink;
		};

		/////////////////////////// Template definition ///////////////////////////

		template<typename ReaderWriter, typename ContextType, typename ReadMessageType>
		RPCRead<ReaderWriter, ContextType, ReadMessageType>::RPCRead(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
			const std::shared_ptr<ghost::ReaderSink>& readerSink)
			: RPCOperation<ReaderWriter, ContextType>(parent, true, false) // restart = true, blocking = false
			, _readerSink(readerSink)
		{
		}

		template<typename ReaderWriter, typename ContextType, typename ReadMessageType>
		RPCRead<ReaderWriter, ContextType, ReadMessageType>::~RPCRead()
		{
			RPCOperation<ReaderWriter, ContextType>::stop();
		}

		template<typename ReaderWriter, typename ContextType, typename ReadMessageType>
		bool RPCRead<ReaderWriter, ContextType, ReadMessageType>::initiateOperation()
		{
			auto rpc = RPCOperation<ReaderWriter, ContextType>::_rpc.lock();
			if (!rpc)
				return false;

			// start reading some stuff
			rpc->getClient()->Read(&_incomingMessage, &(RPCOperation<ReaderWriter, ContextType>::_operationCompletedCallback));
			return true;
		}

		template<typename ReaderWriter, typename ContextType, typename ReadMessageType>
		void RPCRead<ReaderWriter, ContextType, ReadMessageType>::onOperationSucceeded(bool rpcFinished)
		{
			google::protobuf::Any anyMessage;
			if (_incomingMessage.GetTypeName() == anyMessage.descriptor()->full_name())
				anyMessage = _incomingMessage;
			else
				anyMessage.PackFrom(_incomingMessage);
			_readerSink->put(anyMessage);
		}

		template<typename ReaderWriter, typename ContextType, typename ReadMessageType>
		void RPCRead<ReaderWriter, ContextType, ReadMessageType>::onOperationFailed(bool rpcFinished)
		{
			if (rpcFinished)
				return; // nothing to do here

			auto rpc = RPCOperation<ReaderWriter, ContextType>::_rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::INACTIVE);
		}

	}
}

#endif // GHOST_INTERNAL_NETWORK_RPCREAD_HPP
