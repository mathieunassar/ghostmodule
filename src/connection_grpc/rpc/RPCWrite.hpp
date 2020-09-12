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

#include <ghost/connection/WriterSink.hpp>
#include <memory>

#include "RPCOperation.hpp"

namespace ghost
{
namespace internal
{
template <typename ReaderWriter, typename ContextType, typename WriteMessageType>
class RPCWrite : public RPCOperation<ReaderWriter, ContextType>
{
public:
	RPCWrite(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
		 const std::shared_ptr<ghost::WriterSink>& writerSink);

protected:
	bool initiateOperation() override;
	void onOperationSucceeded() override;
	void onOperationFailed() override;

private:
	std::shared_ptr<ghost::WriterSink> _writerSink;
};

/////////////////////////// Template definition ///////////////////////////

template <typename ReaderWriter, typename ContextType, typename WriteMessageType>
RPCWrite<ReaderWriter, ContextType, WriteMessageType>::RPCWrite(std::weak_ptr<RPC<ReaderWriter, ContextType>> parent,
								const std::shared_ptr<ghost::WriterSink>& writerSink)
    : RPCOperation<ReaderWriter, ContextType>(parent), _writerSink(writerSink)
{
}

template <typename ReaderWriter, typename ContextType, typename WriteMessageType>
bool RPCWrite<ReaderWriter, ContextType, WriteMessageType>::initiateOperation()
{
	auto rpc = RPCOperation<ReaderWriter, ContextType>::_rpc.lock();
	if (!rpc) return false;

	google::protobuf::Any message;
	bool success = false;

	bool hasMessage = _writerSink->get(message, std::chrono::milliseconds(0));
	if (!hasMessage) return false;

	WriteMessageType msg;
	if (msg.GetTypeName() == message.descriptor()->full_name()) // Don't unpack any to any because it will fail
		msg = message;
	else
	{
		bool unpackSuccess = message.UnpackTo(&msg);
		if (!unpackSuccess) return false;
	}

	rpc->getClient()->Write(msg, &(RPCOperation<ReaderWriter, ContextType>::_operationCompletedCallback));
	return true;
}

template <typename ReaderWriter, typename ContextType, typename WriteMessageType>
void RPCWrite<ReaderWriter, ContextType, WriteMessageType>::onOperationSucceeded()
{
	auto rpc = RPCOperation<ReaderWriter, ContextType>::_rpc.lock();
	if (!rpc) return;
	if (rpc->isFinished()) return; // nothing to do here

	_writerSink->pop();
}

template <typename ReaderWriter, typename ContextType, typename WriteMessageType>
void RPCWrite<ReaderWriter, ContextType, WriteMessageType>::onOperationFailed()
{
	auto rpc = RPCOperation<ReaderWriter, ContextType>::_rpc.lock();
	if (!rpc) return;
	if (rpc->isFinished()) return; // nothing to do here

	rpc->getStateMachine().setState(RPCStateMachine::INACTIVE);
}

} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_RPCWRITE_HPP
