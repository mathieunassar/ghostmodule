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

#include "RemoteClientGRPC.hpp"

#include "ServerGRPC.hpp"

using namespace ghost::internal;

RemoteClientGRPC::RemoteClientGRPC(const ghost::ConnectionConfiguration& configuration,
				   const std::shared_ptr<ghost::ThreadPool>& threadPool,
				   const std::shared_ptr<IncomingRPC>& rpc, ServerGRPC* parentServer)
    : ghost::Client(configuration), _threadPool(threadPool), _rpc(rpc), _parentServer(parentServer)
{
}

bool RemoteClientGRPC::start()
{
	return false; // it is supposed to be already started by the server
}

bool RemoteClientGRPC::stop()
{
	if (_execution.valid()) _execution.get();
	return !isRunning();
}

bool RemoteClientGRPC::isRunning() const
{
	return !_rpc->isFinished() ||
	       _execution.valid() && _execution.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready;
}

void RemoteClientGRPC::execute()
{
	_execution = _threadPool->execute([this] {
		if (_parentServer->getClientHandler())
			_parentServer->getClientHandler()->configureClient(_rpc->getParent());

		_rpc->startReader(getReaderSink());
		_rpc->startWriter(getWriterSink());

		// call the application code
		bool continueExecution = true;
		bool keepClientAlive = false;

		if (_parentServer->getClientHandler())
			continueExecution =
			    _parentServer->getClientHandler()->handle(_rpc->getParent(), keepClientAlive);

		// only stop the client if the user left "keepClientAlive" to false
		if (!keepClientAlive) shutdown();

		// if continueExecution is false, stop the server
		if (!continueExecution) _parentServer->shutdown();
	});
}

void RemoteClientGRPC::shutdown()
{
	_rpc->stop();
}

std::shared_ptr<ghost::ReaderSink> RemoteClientGRPC::getReaderSink() const
{
	return ghost::ReadableConnection::getReaderSink();
}

std::shared_ptr<ghost::WriterSink> RemoteClientGRPC::getWriterSink() const
{
	return ghost::WritableConnection::getWriterSink();
}

const std::shared_ptr<IncomingRPC> RemoteClientGRPC::getRPC() const
{
	return _rpc;
}
