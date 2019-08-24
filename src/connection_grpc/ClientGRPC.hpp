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

#ifndef GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP

#include <ghost/connection/Client.hpp>
#include <ghost/connection/NetworkConnectionConfiguration.hpp>
#include "rpc/OutgoingRPC.hpp"

namespace ghost
{
	namespace internal
	{
		// RemoteGRPCClient vs GRPCClient (find better names) use RPC + operations for ghost workflow
		// - give the operations an interface which can be faked
		// - RPCRead, RPCReadOne, RPCWrite, RPCWriteOne, RPCConnect, RPCFinsh

		class ClientGRPC : ghost::Client
		{
		public:
			ClientGRPC(const ghost::NetworkConnectionConfiguration& config);

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

		private:
			OutgoingRPC _client;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
