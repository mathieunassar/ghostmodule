/*
 * Copyright 2020 Mathieu Nassar
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

#ifndef GHOST_MESSAGE_HPP
#define GHOST_MESSAGE_HPP

#include <string>

namespace ghost
{
/**
 * @brief Base class for messages that can be sent through a connection.
 *
 * Connection objects expect either a message deriving this base type or
 * a Google protobuf message.
 */
class Message
{
public:
	virtual ~Message() = default;

	/**
	 * @brief accesses the format name.
	 *
	 * The format name is used during the message serialization process.
	 *
	 * @return the format name
	 */
	virtual std::string getMessageFormatName() const = 0;

	/**
	 * @brief accesses the message type name
	 *
	 * @return the type name of the message
	 */
	virtual std::string getMessageTypeName() const = 0;

	/**
	 * @brief Serializes the message into a string.
	 *
	 * @param result the serialized string
	 * @return true if the serialization succeeded.
	 * @return false otherwise
	 */
	virtual bool serialize(std::string& result) const = 0;

	/**
	 * @brief Deserializes the string into the message.
	 *
	 * @param payload the string to deserialize.
	 * @return true if the message was successfully decoded into this object instance
	 * @return false if the deserialization failed.
	 */
	virtual bool deserialize(const std::string& payload) = 0;
};
} // namespace ghost

#endif // GHOST_MESSAGE_HPP
