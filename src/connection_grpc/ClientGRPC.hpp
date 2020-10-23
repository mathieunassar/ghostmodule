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

#ifndef GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP

#include <ghost/connection/Client.hpp>
#include <ghost/connection/NetworkConnectionConfiguration.hpp>
#include <ghost/module/ThreadPool.hpp>

#include "rpc/OutgoingRPC.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Utilizes an ghost::internal::OutgoingRPC to fulfill the ghost::Client interface.
 *	Initialized the RPC with a writerSink and a readerSink from the ghost::ReabableConnection
 *	and ghost::WritableConnection.
 */
class ClientGRPC : public ghost::Client
{
public:
	ClientGRPC(const ghost::ConnectionConfiguration& config, const std::shared_ptr<ghost::ThreadPool>& threadPool);
	ClientGRPC(const ghost::NetworkConnectionConfiguration& config,
		   const std::shared_ptr<ghost::ThreadPool>& threadPool);

	bool start() override;
	bool stop() override;
	bool isRunning() const override;

private:
	OutgoingRPC _client;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
