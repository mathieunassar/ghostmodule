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

#ifndef GHOST_CONNECTION_HPP
#define GHOST_CONNECTION_HPP

#include <ghost/connection/internal/Connection.hpp>

namespace ghost
{
	/**
	 * A connection.
	 * @author	Mathieu Nassar
	 * @date	21.05.2018
	 */
	class Connection : protected internal::Connection
	{
	public:
		Connection(const ConnectionConfiguration& configuration);

		virtual ~Connection() = 0;

		/**
		* If the start sequence is successful, then the connection is running.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		* @return	true if the start sequence was successful, false otherwise.
		*/
		virtual bool start() = 0;

		/**
		* If the stop sequence is successful, then the connection is stopped.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		* @return	true if the stop sequence was successful, false otherwise.
		*/
		virtual bool stop() = 0;

		/**
		* Query if this object is running.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		* @return	true if the connection is currently running, false otherwise.
		*/
		virtual bool isRunning() const = 0;
	};

	inline Connection::~Connection() {}
}

#endif //GHOST_CONNECTION_HPP
