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

#ifndef GHOST_INTERNAL_QUEUEDSINK_HPP
#define GHOST_INTERNAL_QUEUEDSINK_HPP

#include <google/protobuf/any.pb.h>
#include <BlockingQueue.hpp>

namespace ghost
{
	namespace internal
	{
		class QueuedSink
		{
		public:
			QueuedSink()
				: _messageQueue(new BlockingQueue<google::protobuf::Any>()) {}
			virtual ~QueuedSink() = default;

		protected:
			std::shared_ptr<BlockingQueue<google::protobuf::Any>> getMessageQueue()
			{
				return _messageQueue;
			}

		private:
			std::shared_ptr<BlockingQueue<google::protobuf::Any>> _messageQueue;
		};
	}
}

#endif //GHOST_INTERNAL_QUEUEDSINK_HPP
