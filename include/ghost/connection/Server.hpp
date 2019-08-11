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

#ifndef GHOST_SERVER_HPP
#define GHOST_SERVER_HPP

#include <memory>

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/ClientHandler.hpp>

namespace ghost
{
	/**
	 * @brief Server that awaits Client connections.
	 * 
	 * For each incoming connection, the provided implementation of the ClientHandler
	 * will be called. See the ClientHandler documentation for more information.
	 * 
	 * A Server can be created via a ConnectionManager, which will also
	 * manage its lifetime.
	 * 
	 * The configuration provided in the constructor is used to set up the connection
	 * parameters such as the target IP address, port number or threading or blocking
	 * parameters.
	 * 
	 * Particular implementations of Connection classes may define new connection
	 * parameters which can be passed through the ConnectionConfiguration.
	 */
	class Server : public ghost::Connection
	{
	public:
		virtual ~Server() = default;

		/**
		 * @brief Sets the handler for incoming Client connections. The handler
		 * will be called for every connection.
		 * 
		 * @param handler custom implementation of a ClientHandler
		 */
		virtual void setClientHandler(std::shared_ptr<ghost::ClientHandler> handler) = 0;
	};
}

#endif //GHOST_SERVER_HPP
