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

#ifndef GHOST_INTERNAL_NETWORK_READERRPC_HPP
#define GHOST_INTERNAL_NETWORK_READERRPC_HPP

#include <ghost/connection/ReaderSink.hpp>
#include <memory>
#include "RPCRead.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Base class for a reading connection (IncomingRPC and OutgoingRPC).
 *	Manages the RPCRead calls and populates the readerSink with newly received messages.
 */
template <typename ReaderWriter, typename ContextType>
class ReaderRPC
{
public:
	virtual ~ReaderRPC() = default;

	void initReader(std::shared_ptr<RPC<ReaderWriter, ContextType>> rpc,
			const std::shared_ptr<ghost::ReaderSink>& sink = nullptr);
	void startReader(const std::shared_ptr<ghost::ReaderSink>& sink = nullptr);
	void drainReader();
	void stopReader();

private:
	void restartReader();

	std::shared_ptr<RPC<ReaderWriter, ContextType>> _rpc;
	std::mutex _readerMutex;
	std::shared_ptr<RPCRead<ReaderWriter, ContextType, google::protobuf::Any>> _activeReaderOperation;
	std::shared_ptr<RPCRead<ReaderWriter, ContextType, google::protobuf::Any>> _completedReaderOperation;
	std::shared_ptr<ghost::ReaderSink> _readerSink;
};

template <typename ReaderWriter, typename ContextType>
void ReaderRPC<ReaderWriter, ContextType>::initReader(std::shared_ptr<RPC<ReaderWriter, ContextType>> rpc,
						      const std::shared_ptr<ghost::ReaderSink>& sink)
{
	_readerSink = sink;
	_rpc = rpc;
}

template <typename ReaderWriter, typename ContextType>
void ReaderRPC<ReaderWriter, ContextType>::startReader(const std::shared_ptr<ghost::ReaderSink>& sink)
{
	if (sink) _readerSink = sink;

	if (_readerSink)
	{
		auto readerOperation =
		    std::make_shared<RPCRead<ReaderWriter, ContextType, google::protobuf::Any>>(_rpc, _readerSink);

		// Register a callback on completion, so that the operation can be restarted
		readerOperation->onFinish(std::bind(&ReaderRPC<ReaderWriter, ContextType>::restartReader, this));

		// Start the operation
		bool startResult = readerOperation->start();
		if (startResult) _activeReaderOperation = readerOperation;
	}
}

template <typename ReaderWriter, typename ContextType>
void ReaderRPC<ReaderWriter, ContextType>::restartReader()
{
	std::unique_lock<std::mutex> lock(_readerMutex);
	_completedReaderOperation = std::move(_activeReaderOperation);

	auto readerOperation =
	    std::make_shared<RPCRead<ReaderWriter, ContextType, google::protobuf::Any>>(_rpc, _readerSink);
	readerOperation->onFinish(std::bind(&ReaderRPC<ReaderWriter, ContextType>::restartReader, this));

	bool startResult = readerOperation->start();
	if (startResult) _activeReaderOperation = readerOperation;
}

template <typename ReaderWriter, typename ContextType>
void ReaderRPC<ReaderWriter, ContextType>::drainReader()
{
	if (_readerSink) _readerSink->drain();
}

template <typename ReaderWriter, typename ContextType>
void ReaderRPC<ReaderWriter, ContextType>::stopReader()
{
}

} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_READERRPC_HPP
