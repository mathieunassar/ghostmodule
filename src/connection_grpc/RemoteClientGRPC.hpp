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

#ifndef GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP

#include <ghost/connection/Client.hpp>
#include <ghost/module/ThreadPool.hpp>
#include <memory>

#include "rpc/IncomingRPC.hpp"

namespace ghost
{
namespace internal
{
class ServerGRPC;

class RemoteClientGRPC : public ghost::Client
{
public:
	RemoteClientGRPC(const ghost::ConnectionConfiguration& configuration,
			 const std::shared_ptr<ghost::ThreadPool>& threadPool, const std::shared_ptr<IncomingRPC>& rpc,
			 ServerGRPC* parentServer);

	bool start() override;
	bool stop() override;
	bool isRunning() const override;

	void execute();
	void shutdown();

	std::shared_ptr<ghost::ReaderSink> getReaderSink() const;
	std::shared_ptr<ghost::WriterSink> getWriterSink() const;
	const std::shared_ptr<IncomingRPC> getRPC() const;

private:
	std::shared_ptr<ghost::ThreadPool> _threadPool;
	std::future<void> _execution;
	std::shared_ptr<IncomingRPC> _rpc;

	ServerGRPC* _parentServer;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
