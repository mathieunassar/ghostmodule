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

#ifndef GHOST_MESSAGEHANDLER_HPP
#define GHOST_MESSAGEHANDLER_HPP

#include <memory>
#include <functional>
#include <ghost/connection/internal/MessageHandlerCallback.hpp>

namespace ghost
{
	/**
	 * @brief Collection of handlers for incoming messages.
	 * 
	 * The method "addHandler" can be used to set up handlers for specific types
	 * of messages. The type of message is passed as a template parameter to this
	 * call.
	 */
	class MessageHandler
	{
	public:
		virtual ~MessageHandler() = default;

		/**
		 * @brief Adds a handler that processes messages of the templated type.
		 *
		 * If a handler already exists for the given message type, it will be replaced.
		 *
		 * @tparam MessageType the type of messages handled by this handler
		 * @param handler the handler that can handle the messages of this type
		 */
		template<typename MessageType, typename std::enable_if<!std::is_base_of<ghost::Message, MessageType>::value, void>::type* = nullptr>
		void addHandler(std::function<void(const MessageType & message)> handler);
		template<typename MessageType, typename std::enable_if<std::is_base_of<ghost::Message, MessageType>::value, ghost::Message>::type* = nullptr>
		void addHandler(std::function<void(const MessageType & message)> handler);

	protected:
		// the following contains internal implementation detail. Please do not rely on this in your code.

		virtual void addHandler(const std::string& format, const std::string& name,
			std::unique_ptr<ghost::internal::BaseMessageHandlerCallback>&& handler) = 0;
	};

	// TEMPLATE DEFINITION //

	template<typename MessageType, typename std::enable_if<!std::is_base_of<ghost::Message, MessageType>::value, void>::type*>
	void ghost::MessageHandler::addHandler(std::function<void(const MessageType & message)> handler)
	{
		std::string format = ghost::internal::GHOSTMESSAGE_FORMAT_NAME;
		std::string name = MessageType().GetTypeName();
		addHandler(format, name, std::unique_ptr<ghost::internal::MessageHandlerCallback<MessageType>>(new ghost::internal::MessageHandlerCallback<MessageType>(handler)));
	}

	/// Implementation for ghost::Message
	template<typename MessageType, typename std::enable_if<std::is_base_of<ghost::Message, MessageType>::value, ghost::Message>::type*>
	void ghost::MessageHandler::addHandler(std::function<void(const MessageType & message)> handler)
	{
		auto msg = MessageType();
		std::string format = msg.getMessageFormatName();
		std::string name = msg.getMessageTypeName();
		addHandler(format, name, std::unique_ptr<ghost::internal::MessageHandlerCallback<MessageType>>(new ghost::internal::MessageHandlerCallback<MessageType>(handler)));
	}
}

#endif //GHOST_MESSAGEHANDLER_HPP
