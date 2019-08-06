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

#ifndef GHOST_INTERNAL_WRITERSINK_HPP
#define GHOST_INTERNAL_WRITERSINK_HPP

#include <google/protobuf/any.pb.h>
#include <ghost/connection/internal/QueuedSink.hpp>

namespace ghost
{
	namespace internal
	{
		/*
		This will contain the write queue, constructor can take another writer sink to transfer the write queue
		*/
		class WriterSink : public QueuedSink
		{
		public:
			virtual ~WriterSink() = 0;

			/**
			* Gets a message from the writer.
			* @author	Mathieu Nassar
			* @date	21.05.2018
			* @param	message	message to send.
			* @return	A long.
			*/
			virtual bool get(google::protobuf::Any& message, std::chrono::milliseconds timeout) = 0;

			/**
			 *	Remove the last message from the queue. This method must be called after the message
			 *	is effectively sent.
			 */
			virtual void pop() = 0;
		};

		inline WriterSink::~WriterSink() {}
	}
}

#endif //GHOST_INTERNAL_WRITERSINK_HPP
