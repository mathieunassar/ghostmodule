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

#ifndef GHOST_INTERNAL_MESSAGEHANDLER_HPP
#define GHOST_INTERNAL_MESSAGEHANDLER_HPP

#include <string>
#include <map>
#include <memory>
#include <functional>
#include <google/protobuf/any.pb.h>

#include <ghost/connection/Message.hpp>
#include <ghost/connection/MessageHandler.hpp>
#include <ghost/connection/internal/MessageHandlerCallback.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		* Base class for message handlers.
		* @author	Mathieu Nassar
		* @date	15.06.2018
		*/
		class MessageHandler : public ghost::MessageHandler
		{
		public:
			MessageHandler();

			void handle(const google::protobuf::Any& message);

			/// Implementation for protobuf messages
			template<typename MessageType, typename std::enable_if<!std::is_base_of<ghost::Message, MessageType>::value, void>::type* = nullptr>
			void registerHandler(std::function<void(const MessageType& message)> handler)
			{
				std::string format = GHOSTMESSAGE_FORMAT_NAME;
				std::string name = MessageType().GetTypeName();
				_handlers[std::make_pair(format, name)] = std::unique_ptr<MessageHandlerCallback<MessageType>>(new MessageHandlerCallback<MessageType>(handler));
			}

			/// Implementation for ghost::Message
			template<typename MessageType, typename std::enable_if<std::is_base_of<ghost::Message, MessageType>::value, ghost::Message>::type* = nullptr>
			void registerHandler(std::function<void(const MessageType& message)> handler)
			{
				auto msg = MessageType();
				std::string format = msg.getMessageFormatName();
				std::string name = msg.getMessageTypeName();
				_handlers[std::make_pair(format, name)] = std::unique_ptr<MessageHandlerCallback<MessageType>>(new MessageHandlerCallback<MessageType>(handler));
			}
		
		private:
			std::map<std::pair<std::string, std::string>, std::unique_ptr<BaseMessageHandlerCallback>> _handlers;
		};
	}

	template<typename MessageType>
	void MessageHandler::addHandler(std::function<void(const MessageType& message)> handler)
	{
		_internal->registerHandler(handler);
	}
}

#endif //GHOST_INTERNAL_MESSAGEHANDLER_HPP
