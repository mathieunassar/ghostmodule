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

#ifndef GHOST_CLIENT_HPP
#define GHOST_CLIENT_HPP

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/ReadableConnection.hpp>
#include <ghost/connection/WritableConnection.hpp>

namespace ghost
{
	/**
	 * @brief Client that connects to a Server.
	 * 
	 * A Client can be created via a ConnectionManager, which will also
	 * manage its lifetime.
	 * 
	 * The configuration provided in the constructor is used to set up the connection
	 * parameters such as the target IP address, port number or threading or blocking
	 * parameters.
	 * 
	 * Particular implementations of Connection classes may define new connection
	 * parameters which can be passed through the ConnectionConfiguration.
	 *
	 * It is possible to read and write through client connections.
	 */
	class Client : public ghost::Connection, public ghost::ReadableConnection, public ghost::WritableConnection
	{
	public:
		virtual ~Client() = default;
	};
}

#endif //GHOST_CLIENT_HPP
