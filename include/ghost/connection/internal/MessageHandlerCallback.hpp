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

#ifndef GHOST_INTERNAL_MESSAGEHANDLERCALLBACK_HPP
#define GHOST_INTERNAL_MESSAGEHANDLERCALLBACK_HPP

#include <google/protobuf/any.pb.h>

#include <functional>
#include <ghost/connection/Message.hpp>

#include "GenericMessageConverter.hpp"
#include "ProtobufMessage.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Base class for the handlers
 */
class BaseMessageHandlerCallback
{
public:
	virtual ~BaseMessageHandlerCallback() = default;
	virtual void handle(const google::protobuf::Any& message) = 0;
};

template <typename MessageType, bool = std::is_base_of<ghost::Message, MessageType>::value>
class MessageHandlerCallback;

/// Implementation for protobuf messages
template <typename MessageType>
class MessageHandlerCallback<MessageType, false> : public BaseMessageHandlerCallback
{
public:
	MessageHandlerCallback(std::function<void(const MessageType& message)> callback) : _callback(callback)
	{
	}

	void handle(const google::protobuf::Any& message) override
	{
		auto proto = std::make_shared<MessageType>();
		ghost::internal::ProtobufMessage msg(proto);
		bool parseSuccess = GenericMessageConverter::parse(message, msg);
		if (parseSuccess) _callback(*proto);
	}

private:
	std::function<void(const MessageType& message)> _callback;
};

/// Implementation for ghost::Message
template <typename MessageType>
class MessageHandlerCallback<MessageType, true> : public BaseMessageHandlerCallback
{
public:
	MessageHandlerCallback(std::function<void(const MessageType& message)> callback) : _callback(callback)
	{
	}

	void handle(const google::protobuf::Any& message) override
	{
		MessageType msg;
		bool parseSuccess = GenericMessageConverter::parse(message, msg);
		if (parseSuccess) _callback(msg);
	}

private:
	std::function<void(const MessageType& message)> _callback;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_MESSAGEHANDLERCALLBACK_HPP
