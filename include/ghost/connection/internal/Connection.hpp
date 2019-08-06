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

#ifndef GHOST_INTERNAL_CONNECTION_HPP
#define GHOST_INTERNAL_CONNECTION_HPP

#include <memory>

#include <ghost/connection/ConnectionConfiguration.hpp>
#include <ghost/connection/internal/GenericWriter.hpp>
#include <ghost/connection/internal/GenericReader.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		* The base class for any connection.
		* Handles the writer and reader sinks.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		*/
		class Connection
		{
		public:
			Connection(const ConnectionConfiguration& configuration);
			virtual ~Connection() = 0;

			template<typename MessageType>
			std::shared_ptr<Writer<MessageType>> getWriter();

			template<typename MessageType>
			std::shared_ptr<Reader<MessageType>> getReader();

		protected:
			/// a new sink is available - default implementation: replace the member variable
			virtual void setWriterSink(std::shared_ptr<WriterSink> sink);
			/// a new sink is available - default implementation: replace the member variable
			virtual void setReaderSink(std::shared_ptr<ReaderSink> sink);
			/// returns a shared_ptr to the currently configured writer sink
			std::shared_ptr<WriterSink> getWriterSink();
			/// returns a shared_ptr to the currently configured reader sink
			std::shared_ptr<ReaderSink> getReaderSink();

			/// copies the current reader sink to the other connection
			void forwardReaderSink(Connection& to);
			/// copies the current writer sink to the other connection
			void forwardWriterSink(Connection& to);

		private:
			ConnectionConfiguration _configuration;
			std::shared_ptr<WriterSink> _writer;
			std::shared_ptr<ReaderSink> _reader;
		};

		inline Connection::~Connection() {}

		#include "Connection.impl.hpp"
	}
}

#endif //GHOST_CONNECTION_HPP
