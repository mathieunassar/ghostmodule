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

#ifndef GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP
#define GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP

#include <deque>
#include <ghost/connection/Client.hpp>
#include <ghost/connection/ClientHandler.hpp>
#include <ghost/connection/Writer.hpp>
#include <mutex>

namespace ghost
{
namespace internal
{
/**
 *	This handler keeps the clients which connect to the server, and sends them the published data.
 */
class PublisherClientHandler : public ghost::ClientHandler
{
public:
	~PublisherClientHandler();

	bool handle(std::shared_ptr<ghost::Client> client, bool& keepClientAlive) override;

	bool send(const google::protobuf::Any& message);
	void releaseClients();
	size_t countSubscribers() const;

private:
	mutable std::mutex _subscribersMutex;
	std::deque<std::pair<std::shared_ptr<ghost::Client>, std::shared_ptr<ghost::Writer<google::protobuf::Any>>>>
	    _subscribers;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_PUBLISHERCLIENTHANDLER_HPP
