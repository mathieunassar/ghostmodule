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

#ifndef GHOST_INTERNAL_GENERICWRITER_HPP
#define GHOST_INTERNAL_GENERICWRITER_HPP

#include <ghost/connection/Message.hpp>
#include <ghost/connection/Writer.hpp>
#include <memory>
#include <type_traits>

#include "GenericMessageConverter.hpp"
#include "ProtobufMessage.hpp"

namespace ghost
{
namespace internal
{
template <typename MessageType>
class GenericWriter : public ghost::Writer<MessageType>
{
public:
	/// Constructor. The value of blocking determines whether calls to "write" will
	/// be blocking or not.
	GenericWriter(const std::shared_ptr<ghost::WriterSink>& sink, bool blocking)
	    : _internal(ghost::Writer<google::protobuf::Any>::create(sink, blocking))
	{
	}

	bool write(const MessageType& message) override;

private:
	template <class Q = MessageType>
	typename std::enable_if<std::is_same<google::protobuf::Any, Q>::value, bool>::type makeAny(
	    google::protobuf::Any& any, const MessageType& message)
	{
		any = message;
		return true;
	}

	template <class Q = MessageType>
	typename std::enable_if<
	    !std::is_base_of<ghost::Message, Q>::value && !std::is_same<google::protobuf::Any, Q>::value, bool>::type
	makeAny(google::protobuf::Any& any, const MessageType& message)
	{
		return GenericMessageConverter::create(
		    any, ghost::internal::ProtobufMessage(std::make_shared<MessageType>(message)));
	}

	template <class Q = MessageType>
	typename std::enable_if<std::is_base_of<ghost::Message, Q>::value, bool>::type makeAny(
	    google::protobuf::Any& any, const MessageType& message)
	{
		return GenericMessageConverter::create(any, message);
	}

	std::shared_ptr<ghost::Writer<google::protobuf::Any>> _internal;
};

// TEMPLATE DEFINITION //

template <typename MessageType>
bool GenericWriter<MessageType>::write(const MessageType& message)
{
	google::protobuf::Any any;

	bool createSuccess = makeAny(any, message);

	if (!createSuccess)
		return false; // conversion failed, i.e. there is no protobuf message in the input or the serialization
			      // failed (in case of a user format)

	return _internal->write(any);
}
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_GENERICWRITER_HPP
