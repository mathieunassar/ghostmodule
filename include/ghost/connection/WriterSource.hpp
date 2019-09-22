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

#ifndef GHOST_WRITERSOURCE_HPP
#define GHOST_WRITERSOURCE_HPP

#include <google/protobuf/any.pb.h>

namespace ghost
{
	/**
	 *	A ghost::WriterSource is the interface between a connection and a
	 *	ghost::Writer object that can write messages to the connection.
	 *	The sink is used by the ghost::Writer object to receive message from the
	 *	matching sink.
	 */
	class WriterSource
	{
	public:
		virtual ~WriterSource() = default;

		/**
		 *	Forwards a message to the sink.
		 *	@param message	Message to push.
		 */
		virtual void push(const google::protobuf::Any& message, bool blocking) = 0;
	};
}

#endif //GHOST_WRITERSOURCE_HPP
