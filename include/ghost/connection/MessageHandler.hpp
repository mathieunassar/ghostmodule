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

namespace ghost
{
	namespace internal
	{
		class MessageHandler;
	}

	/**
	 * @brief Collection of handlers for incoming messages.
	 * 
	 * An implementation of this class can be obtained by calling
	 * "addMessageHandler" on Writer objects. Calling this method will forward
	 * all new received messages to this message handler.
	 * 
	 * The method "addHandler" can be used to set up handlers for specific types
	 * of messages. The type of message is passed as a template parameter to this
	 * call.
	 */
	class MessageHandler
	{
	public:
		virtual ~MessageHandler() = 0;

		/**
		 * @brief Adds a handler that processes messages of the templated type.
		 * 
		 * If a handler already exists for the given message type, it will be replaced.
		 * 
		 * @tparam MessageType the type of messages handled by this handler
		 * @param handler the handler that can handle the messages of this type
		 */
		template<typename MessageType>
		void addHandler(std::function<void(const MessageType& message)> handler);

	protected:
		internal::MessageHandler* _internal;
	};

	inline MessageHandler::~MessageHandler() {}
}

#endif //GHOST_MESSAGEHANDLER_HPP
