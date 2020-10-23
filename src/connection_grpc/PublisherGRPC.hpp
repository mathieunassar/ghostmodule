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

#ifndef GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP
#define GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP

#include <atomic>
#include <ghost/connection/Publisher.hpp>
#include <ghost/module/ThreadPool.hpp>
#include <memory>

#include "PublisherClientHandler.hpp"
#include "ServerGRPC.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Starts a ghost::internal::ServerGRPC to listen to incoming ghost::internal::RemoteClientGRPC
 *	and stores them in its ghost::internal::PublisherClientHandler.
 *	Uses the ghost::ReaderSink from the ghost::WritableConnection to get messages and sends them
 *	to all the registered clients.
 *
 *	Periodically checks for new messages (10ms fixed rate) and reads messages until
 *	the sink is empty when messages are available.
 */
class PublisherGRPC : public ghost::Publisher
{
public:
	PublisherGRPC(const ghost::ConnectionConfiguration& config,
		      const std::shared_ptr<ghost::ThreadPool>& threadPool);
	PublisherGRPC(const ghost::NetworkConnectionConfiguration& config,
		      const std::shared_ptr<ghost::ThreadPool>& threadPool);

	bool start() override;
	bool stop() override;
	bool isRunning() const override;

	size_t countSubscribers() const;

private:
	void writerThread(); // waits for the writer to be fed and sends the data to the handler

	std::shared_ptr<ghost::ThreadPool> _threadPool;
	std::shared_ptr<ghost::ScheduledExecutor> _executor;
	ServerGRPC _server;
	std::shared_ptr<PublisherClientHandler> _handler;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP
