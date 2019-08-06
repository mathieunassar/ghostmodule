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

#ifndef GHOST_INTERNAL_READERSINK_HPP
#define GHOST_INTERNAL_READERSINK_HPP

#include <google/protobuf/any.pb.h>
#include <BlockingQueue.hpp>
#include <ghost/connection/internal/QueuedSink.hpp>

namespace ghost
{
	namespace internal
	{
		/*
		This will contain the read queue, will be initialized at the beginning of a connection
		constructor can take another sink to transfer the queue from one to another
		Create MessageQueue class to handle the queue with its mutex
		*/
		class ReaderSink : public QueuedSink
		{
		public:
			virtual ~ReaderSink() = 0;

			/**
			* Puts a message in the reader.
			* @author	Mathieu Nassar
			* @date	21.05.2018
			* @param	message	message to send.
			* @return	A long.
			*/
			virtual bool put(const google::protobuf::Any& message) = 0;
		};

		inline ReaderSink::~ReaderSink() {}
	}
}

#endif //GHOST_INTERNAL_READERSINK_HPP
