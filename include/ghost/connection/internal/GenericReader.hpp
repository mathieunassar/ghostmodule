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

#ifndef GHOST_INTERNAL_GENERICREADER_HPP
#define GHOST_INTERNAL_GENERICREADER_HPP

#include <google/protobuf/any.pb.h>

#include <ghost/connection/Reader.hpp>
#include <ghost/connection/ReaderSink.hpp>
#include <memory>
#include <type_traits>

#include "GenericMessageConverter.hpp"
#include "ProtobufMessage.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Implementation of a reader, that can read messages of the given templated type.
 *	The reader gets the message from the ReaderSink interface, which is controlled
 *	by a connection.
 *	Its implementation of the Reader interface provides the API functionality.
 */
template <typename MessageType>
class GenericReader : public ghost::Reader<MessageType>
{
public:
	// Constructor. Initializes the internal implementation, which is bound to the internal sink.
	GenericReader(const std::shared_ptr<ghost::ReaderSink>& sink, bool blocking)
	    : _internal(ghost::Reader<google::protobuf::Any>::create(sink, blocking))
	{
	}

	// From ghost::Reader<MessageType>
	bool read(MessageType& message) override;
	bool lastRead(MessageType& message) override;

private:
	template <class Q = MessageType>
	typename std::enable_if<!std::is_base_of<ghost::Message, Q>::value, bool>::type makeMessage(
	    const google::protobuf::Any& any, MessageType& message)
	{
		auto proto = std::make_shared<MessageType>();
		ghost::internal::ProtobufMessage container(proto);
		bool parseResult = GenericMessageConverter::parse(any, container);
		message = *proto;
		return parseResult;
	}

	template <class Q = MessageType>
	typename std::enable_if<std::is_base_of<ghost::Message, Q>::value, bool>::type makeMessage(
	    const google::protobuf::Any& any, MessageType& message)
	{
		return GenericMessageConverter::parse(any, message);
	}

	std::shared_ptr<ghost::Reader<google::protobuf::Any>> _internal;
};

// TEMPLATE DEFINITION //

template <typename MessageType>
bool GenericReader<MessageType>::read(MessageType& message)
{
	google::protobuf::Any tmp;
	bool readResult = _internal->read(tmp);
	if (!readResult) return false;

	return makeMessage(tmp, message);
}

template <typename MessageType>
bool GenericReader<MessageType>::lastRead(MessageType& message)
{
	google::protobuf::Any tmp;
	bool readResult = _internal->lastRead(tmp);
	if (!readResult) return false;

	return makeMessage(tmp, message);
}
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_GENERICREADER_HPP
