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

#ifndef GHOST_READABLECONNECTION_HPP
#define GHOST_READABLECONNECTION_HPP

#include <memory>
#include <ghost/connection/Reader.hpp>
#include <ghost/connection/ReaderSink.hpp>
#include <ghost/connection/MessageHandler.hpp>
#include <ghost/connection/ConnectionConfiguration.hpp>

namespace ghost
{
	class ReadableConnection
	{
	public:
		ReadableConnection(const ghost::ConnectionConfiguration& configuration);
		virtual ~ReadableConnection() = default;

		/**
		 * @brief Adds a message handler to the connection and returns it.
		 *
		 * See the MessageHandler documentation for more information. If a message
		 * handler is set to this connection, subsequent calls to "read" will return
		 * false.
		 *
		 * After the message handler was added, handler functions can be added to it.
		 *
		 * @return the message handler that was added to the reader.
		 */
		std::shared_ptr<ghost::MessageHandler> addMessageHandler();

		/**
		 *	Creates a reader for this connection.
		 *	@return a reader configured for this connection.
		 */
		template<typename MessageType>
		std::shared_ptr<ghost::Reader<MessageType>> getReader() const
		{
			return ghost::Reader<MessageType>::create(_readerSink, _blocking);
		}

	protected:
		/**
		 *	Gets the ghost::ReaderSink of this connection.
		 *	This method shall be used by implementations to push messages to this
		 *	connection's readers.
		 */
		std::shared_ptr<ghost::ReaderSink> getReaderSink() const;

	private:
		std::shared_ptr<ghost::ReaderSink> _readerSink;
		bool _blocking;
	};
}

#endif //GHOST_READABLECONNECTION_HPP
