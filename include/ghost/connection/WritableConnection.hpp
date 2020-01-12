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

#ifndef GHOST_WRITABLECONNECTION_HPP
#define GHOST_WRITABLECONNECTION_HPP

#include <ghost/connection/ConnectionConfiguration.hpp>
#include <ghost/connection/Writer.hpp>
#include <ghost/connection/WriterSink.hpp>
#include <memory>

namespace ghost
{
/**
 *	A ghost::WritableConnection is a connection that can send messages.
 *	This class provides a method to create a ghost::Writer, responsible for
 *	forwarding outgoing messages to the connection's implementation.
 */
class WritableConnection
{
public:
	WritableConnection(const ghost::ConnectionConfiguration& configuration);
	virtual ~WritableConnection() = default;

	/**
	 *	Creates a writer for this connection.
	 *	@return a writer configured for this connection.
	 */
	template <typename MessageType>
	std::shared_ptr<ghost::Writer<MessageType>> getWriter() const
	{
		return ghost::Writer<MessageType>::create(_writerSink, _blocking);
	}

protected:
	/**
	 *	Gets the ghost::WriterSink of this connection.
	 *	This method shall be used by implementations to get input from this
	 *	connection's writers.
	 */
	std::shared_ptr<ghost::WriterSink> getWriterSink() const;

private:
	std::shared_ptr<ghost::WriterSink> _writerSink;
	bool _blocking;
};
} // namespace ghost

#endif // GHOST_READABLECONNECTION_HPP
