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

#ifndef GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP
#define GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP

#include <memory>
#include <thread>
#include <atomic>

#include <ghost/connection/Publisher.hpp>
#include "ServerGRPC.hpp"
#include "PublisherClientHandler.hpp"

namespace ghost
{
	namespace internal
	{
		class PublisherGRPC : public ghost::Publisher
		{
		public:
			PublisherGRPC(const ghost::ConnectionConfiguration& config);
			PublisherGRPC(const ghost::NetworkConnectionConfiguration& config);

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

		private:
			void writerThread(); // waits for the writer to be fed and sends the data to the handler

			ServerGRPC _server;
			std::shared_ptr<PublisherClientHandler> _handler;
			std::thread _writerThread;
			std::atomic_bool _writerThreadEnable;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_PUBLISHERGRPC_HPP
