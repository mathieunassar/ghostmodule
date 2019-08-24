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

#ifndef GHOST_INTERNAL_NETWORK_RPCWRITE_HPP
#define GHOST_INTERNAL_NETWORK_RPCWRITE_HPP

#include <memory>

#include <ghost/connection/WriterSink.hpp>

#include "RPCOperation.hpp"

namespace ghost
{
	namespace internal
	{
		template<typename ReaderWriter, typename ContextType, typename WriteMessageType>
		class RPCWrite : public RPCOperation<ReaderWriter, ContextType>
		{
		public:
			RPCWrite(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
				bool autoRestart, bool blocking,
				const std::shared_ptr<ghost::WriterSink>& writerSink);

		protected:
			bool initiateOperation() override;
			void onOperationFailed() override;

		private:
			std::shared_ptr<ghost::WriterSink> _writerSink;
		};

		/////////////////////////// Template definition ///////////////////////////

		template<typename ReaderWriter, typename ContextType, typename WriteMessageType>
		RPCWrite<ReaderWriter, ContextType, WriteMessageType>::RPCWrite(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
			bool autoRestart, bool blocking,
			const std::shared_ptr<ghost::WriterSink>& writerSink)
			: RPCOperation(parent, autoRestart, blocking)
			, _writerSink(writerSink)
		{
		}

		template<typename ReaderWriter, typename ContextType, typename WriteMessageType>
		bool RPCWrite<ReaderWriter, ContextType, WriteMessageType>::initiateOperation()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return false;

			google::protobuf::Any message;
			bool success = false;

			if (_blocking)
			{
				while (!success && rpc->getStateMachine().getState() == RPCStateMachine::EXECUTING)
				{
					success = _writerSink->get(message, std::chrono::milliseconds(1));
				}
			}
			else // Non-blocking call - returns immediatly even if there is no messages in the queue
				success = _writerSink->get(message, std::chrono::milliseconds(0));

			if (success)
			{
				WriteMessageType msg;
				bool unpackSuccess = message.UnpackTo(&msg);
				if (!unpackSuccess)
					return false;

				rpc->getClient()->Write(msg, &_operationCompletedCallback);
			}

			return success;
		}

		template<typename ReaderWriter, typename ContextType, typename WriteMessageType>
		void RPCWrite<ReaderWriter, ContextType, WriteMessageType>::onOperationFailed()
		{
			auto rpc = _rpc.lock();
			if (!rpc)
				return;

			rpc->getStateMachine().setState(RPCStateMachine::INACTIVE);
		}

	}
}

#endif // GHOST_INTERNAL_NETWORK_RPCWRITE_HPP
