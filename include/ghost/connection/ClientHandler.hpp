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

#ifndef GHOST_CLIENTHANDLER_HPP
#define GHOST_CLIENTHANDLER_HPP

#include <memory>
#include <ghost/connection/Client.hpp>

namespace ghost
{
	/**
	 * @brief Handler for a new client connection.
	 * 
	 * Implementations of this base class will be called for each new client connection.
	 * 
	 * The handle method will specifically be called with a Client object passed,
	 * which represents the remote client connection.
	 * 
	 * A client handler may choose to keep the connection to the client alive by modifying
	 * the value of "keepClientAlive" to true.
	 * 
	 * A client handler may also shut down the Server by returning false on the call to
	 * "handle".
	 */
	class ClientHandler
	{
	public:
		virtual ~ClientHandler() = default;

		/**
		 * @brief Provides the possibility to configure the message handler of the client
		 * before it is started.
		 * @param client represents the client connection. At that point, the connection is
		 * established but the readers and writers are not started yet.
		 */
		virtual void configureClient(const std::shared_ptr<Client>& client) {}

		/**
		 * @brief Handles a client connection.
		 * 
		 * @param client represents the client connection. A Reader or a Writer may be
		 * created from the client to communicate.
		 * @param keepClientAlive set it to true to prevent the server from closing this
		 * connection. Note that if the server is shut down, all connections will be closed
		 * @return true if the server should continue to run
		 * @return false to shutdown the server
		 */
		virtual bool handle(std::shared_ptr<Client> client, bool& keepClientAlive) = 0;
	};
}

#endif //GHOST_CLIENTHANDLER_HPP
