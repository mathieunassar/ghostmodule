/*
 * Copyright 2020 Mathieu Nassar
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

#ifndef GHOST_INTERNAL_NETWORK_WRITERRPC_HPP
#define GHOST_INTERNAL_NETWORK_WRITERRPC_HPP

#include <ghost/connection/WriterSink.hpp>
#include <ghost/module/ThreadPool.hpp>
#include <memory>
#include "RPCWrite.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Base class for a writing connection (IncomingRPC and OutgoingRPC).
 *	Manages the RPCWrite calls and creates them when messages to be sent are received through
 *	the writerSink.
 */
template <typename ReaderWriter, typename ContextType>
class WriterRPC
{
public:
	WriterRPC(const std::shared_ptr<ghost::ThreadPool>& threadPool);
	virtual ~WriterRPC() = default;

	void initWriter(std::shared_ptr<RPC<ReaderWriter, ContextType>> rpc,
			const std::shared_ptr<ghost::WriterSink>& sink = nullptr);
	void startWriter(const std::shared_ptr<ghost::WriterSink>& sink = nullptr);
	void drainWriter();
	void stopWriter();

private:
	void startWriterTask();
	void restartWriter();

	std::shared_ptr<RPC<ReaderWriter, ContextType>> _rpc;
	std::shared_ptr<ghost::WriterSink> _writerSink;
	std::shared_ptr<ghost::ThreadPool> _threadPool;
	std::shared_ptr<ghost::ScheduledExecutor> _startWriterExecutor;
	std::mutex _writerMutex;
	std::shared_ptr<RPCWrite<ReaderWriter, ContextType, google::protobuf::Any>> _activeWriterOperation;
	std::shared_ptr<RPCWrite<ReaderWriter, ContextType, google::protobuf::Any>> _completedWriterOperation;
};

/// template definition

template <typename ReaderWriter, typename ContextType>
WriterRPC<ReaderWriter, ContextType>::WriterRPC(const std::shared_ptr<ghost::ThreadPool>& threadPool)
    : _threadPool(threadPool)
{
}

template <typename ReaderWriter, typename ContextType>
void WriterRPC<ReaderWriter, ContextType>::initWriter(std::shared_ptr<RPC<ReaderWriter, ContextType>> rpc,
						      const std::shared_ptr<ghost::WriterSink>& sink)
{
	_writerSink = sink;
	_rpc = rpc;
}

template <typename ReaderWriter, typename ContextType>
void WriterRPC<ReaderWriter, ContextType>::startWriter(const std::shared_ptr<ghost::WriterSink>& sink)
{
	if (sink) _writerSink = sink;

	if (_writerSink)
	{
		// Create an executor that checks that there is something to read, and create a RPCWrite if necessary
		_startWriterExecutor = _threadPool->makeScheduledExecutor();
		_startWriterExecutor->scheduleAtFixedRate(std::bind(&WriterRPC::startWriterTask, this),
							  std::chrono::milliseconds(10));
	}
}

template <typename ReaderWriter, typename ContextType>
void WriterRPC<ReaderWriter, ContextType>::drainWriter()
{
	if (_writerSink) _writerSink->drain();
}

template <typename ReaderWriter, typename ContextType>
void WriterRPC<ReaderWriter, ContextType>::stopWriter()
{
	if (_startWriterExecutor) _startWriterExecutor->stop();
}

template <typename ReaderWriter, typename ContextType>
void WriterRPC<ReaderWriter, ContextType>::startWriterTask()
{
	std::unique_lock<std::mutex> lock(_writerMutex);

	// Don't start anything if something is already in progress
	if (_activeWriterOperation) return;

	google::protobuf::Any message;

	// Check if there are some messages to send
	bool hasMessage = _writerSink->get(message, std::chrono::milliseconds(0));
	if (!hasMessage) return;

	auto writerOperation =
	    std::make_shared<RPCWrite<ReaderWriter, ContextType, google::protobuf::Any>>(_rpc, _writerSink);

	// Register a callback on completion, so that the operation can be restarted
	writerOperation->onFinish(std::bind(&WriterRPC<ReaderWriter, ContextType>::restartWriter, this));

	// Start the operation
	bool startResult = writerOperation->start();
	if (startResult) _activeWriterOperation = writerOperation;
}

template <typename ReaderWriter, typename ContextType>
void WriterRPC<ReaderWriter, ContextType>::restartWriter()
{
	std::unique_lock<std::mutex> lock(_writerMutex);
	_completedWriterOperation = std::move(_activeWriterOperation);

	auto writerOperation =
	    std::make_shared<RPCWrite<ReaderWriter, ContextType, google::protobuf::Any>>(_rpc, _writerSink);
	writerOperation->onFinish(std::bind(&WriterRPC<ReaderWriter, ContextType>::restartWriter, this));

	bool startResult = writerOperation->start();
	if (startResult) _activeWriterOperation = writerOperation;
}

} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_WRITERRPC_HPP
