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

#ifndef GHOST_INTERNAL_READER_HPP
#define GHOST_INTERNAL_READER_HPP

#include <ghost/connection/Reader.hpp>
#include "ReaderSink.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		 *	The internal implementation of the ghost::Reader is a specialized
		 *	type handling google::protobuf::Any messages in order to get them
		 *	from the ReaderSink object.
		 */
		class Reader : public ghost::Reader<google::protobuf::Any>
		{
		public:
			Reader(const std::shared_ptr<ghost::ReaderSink>& sink, bool blocking);

			bool read(google::protobuf::Any& message) override;
			bool lastRead(google::protobuf::Any& message) override;

		private:
			std::shared_ptr<ReaderSink> _readerSink;
			bool _blocking;
			
			google::protobuf::Any _last;
		};
	}

}

#endif // GHOST_INTERNAL_READER_HPP
