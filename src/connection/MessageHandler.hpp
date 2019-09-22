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
#include <google/protobuf/any.pb.h>
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
			void handle(const google::protobuf::Any& message);

		protected:
			void addHandler(const std::string& format, const std::string& name,
				std::unique_ptr<ghost::internal::BaseMessageHandlerCallback>&& handler) override;
		
		private:
			std::map<std::pair<std::string, std::string>, std::unique_ptr<BaseMessageHandlerCallback>> _handlers;
		};
	}
}

#endif //GHOST_INTERNAL_MESSAGEHANDLER_HPP
