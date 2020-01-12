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

#ifndef GHOST_READER_HPP
#define GHOST_READER_HPP

#include <ghost/connection/ReaderSink.hpp>
#include <memory>

namespace ghost
{
/**
 * @brief Interface for reading through a connection.
 *
 * Reader objects are obtained from connection objects.
 *
 * The type of message being read by the Reader is provided
 * by the template parameter "MessageType", which can be one of
 * the following:
 * - a Google Protobuf message type (including google.protobuf.Any)
 * - A user-defined object type derived from the Message class.
 *
 * @tparam MessageType the type of messages that will be read
 * by the Reader.
 */
template <typename MessageType>
class Reader
{
public:
	virtual ~Reader() = default;

	/**
	 *	Creates a ghost::Reader bound to the provided connection.
	 *	@return a ghost::Reader object that can read from the provided connection.
	 */
	static std::shared_ptr<ghost::Reader<MessageType>> create(const std::shared_ptr<ghost::ReaderSink>& sink,
								  bool blocking);

	/**
	 * @brief Reads a message from the reader.
	 *
	 * If a message handler was added to the connection bound to this reader,
	 * then calling read will always return false.
	 * Read will also return false if the connection was set to not block during
	 * I/O calls.
	 * Finally, the method will return false if the incoming message does not match
	 * the expected type MessageType.
	 * In all the other cases, this method returns true.
	 *
	 * @param message the message to read (input)
	 * @return true if a message of the given type was actually read
	 * @return false if no message was read during this call.
	 */
	virtual bool read(MessageType& message) = 0;

	/**
	 * @brief Gets the last read message.
	 *
	 * @param message the message to read (input)
	 * @return true if a message of the given type was actually read
	 * @return false if no message could be returned
	 */
	virtual bool lastRead(MessageType& message) = 0;
};

template <>
std::shared_ptr<ghost::Reader<google::protobuf::Any>> ghost::Reader<google::protobuf::Any>::create(
    const std::shared_ptr<ghost::ReaderSink>& sink, bool blocking);
} // namespace ghost

#include <ghost/connection/internal/GenericReader.hpp>

// Template definition //
template <typename MessageType>
std::shared_ptr<ghost::Reader<MessageType>> ghost::Reader<MessageType>::create(
    const std::shared_ptr<ghost::ReaderSink>& sink, bool blocking)
{
	return std::make_shared<ghost::internal::GenericReader<MessageType>>(sink, blocking);
}

#endif // GHOST_WRITER_HPP
