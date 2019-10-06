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

#include "MessageHandler.hpp"

#include <ghost/connection/internal/GenericMessageConverter.hpp>

using namespace ghost::internal;

void MessageHandler::handle(const google::protobuf::Any& message)
{
	std::pair<std::string, std::string> formatAndName = GenericMessageConverter::getFormatAndName(message);

	if (_handlers.count(formatAndName)) _handlers.at(formatAndName)->handle(message);
}

void MessageHandler::addHandler(const std::string& format, const std::string& name,
				std::unique_ptr<ghost::internal::BaseMessageHandlerCallback>&& handler)
{
	_handlers[std::make_pair(format, name)] = std::move(handler);
}
