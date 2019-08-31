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

#ifndef GHOST_TESTS_CONNECTIONMOCKS_HPP
#define GHOST_TESTS_CONNECTIONMOCKS_HPP

#include <gmock/gmock.h>

#include <ghost/connection/ClientHandler.hpp>
#include <ghost/connection/Message.hpp>

class ClientHandlerMock : public ghost::ClientHandler
{
public:
	MOCK_METHOD1(configureClient, void(const std::shared_ptr<ghost::Client>& client));
	MOCK_METHOD2(handle, bool(std::shared_ptr<ghost::Client> client, bool& keepClientAlive));
};

class MessageMock : public ghost::Message
{
public:
	MOCK_CONST_METHOD0(getMessageFormatName, std::string());
	MOCK_CONST_METHOD0(getMessageTypeName, std::string());
	MOCK_CONST_METHOD1(serialize, bool(std::string& result));
	MOCK_METHOD1(deserialize, bool(const std::string& payload));
};

#endif // GHOST_TESTS_CONNECTIONMOCKS_HPP
