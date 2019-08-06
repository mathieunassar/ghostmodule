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

#ifndef GHOST_SUBSCRIBER_HPP
#define GHOST_SUBSCRIBER_HPP

#include <memory>

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/Reader.hpp>

namespace ghost
{
	/**
	 * @brief Specific type of Connection to receive messages from Publisher connections.
	 * 
	 * A Subscriber can be created via a ConnectionManager, which will also
	 * manage its lifetime.
	 * 
	 * The configuration provided in the constructor is used to set up the connection
	 * parameters such as the target IP address, port number or threading or blocking
	 * parameters.
	 * 
	 * Particular implementations of Connection classes may define new connection
	 * parameters which can be passed through the ConnectionConfiguration.
	 */
	class Subscriber : public Connection
	{
	public:
		/**
		 * @brief Construct a Subscriber with the provided configuration.
		 * 
		 * @param configuration subscriber's configuration
		 */
		Subscriber(const ConnectionConfiguration& configuration);

		virtual ~Subscriber() = 0;

		/**
		 * @brief gets a Reader from the connection that reads messages of the provided
		 * type.
		 * 
		 * @tparam MessageType Message type read by the Reader objects.
		 * @return the requested reader, which will be fed by this connection.
		 */
		template<typename MessageType>
		std::shared_ptr<Reader<MessageType>> getReader() // same as: using internal::Connection::getReader;
		{
			return internal::Connection::getReader<MessageType>();
		}
	};

	inline Subscriber::~Subscriber() {}
}

#endif // GHOST_SUBSCRIBER_HPP
