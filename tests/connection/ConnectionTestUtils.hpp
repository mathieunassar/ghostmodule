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

#ifndef GHOST_TESTS_CONNECTIONTESTUTILS_HPP
#define GHOST_TESTS_CONNECTIONTESTUTILS_HPP

#include <gmock/gmock.h>

#include <ghost/connection/Client.hpp>
#include <ghost/connection/ClientHandler.hpp>
#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection/Message.hpp>
#include <ghost/connection/Publisher.hpp>
#include <ghost/connection/ReadableConnection.hpp>
#include <ghost/connection/Server.hpp>
#include <ghost/connection/Subscriber.hpp>
#include <ghost/connection/WritableConnection.hpp>

class ServerMock : public ghost::Server
{
public:
	ServerMock(const ghost::ConnectionConfiguration& config);

	MOCK_METHOD0(start, bool());
	MOCK_METHOD0(stop, bool());
	MOCK_CONST_METHOD0(isRunning, bool());
	MOCK_METHOD1(setClientHandler, void(std::shared_ptr<ghost::ClientHandler> handler));
};

class NotRunningServerMock : public ServerMock
{
public:
	NotRunningServerMock(const ghost::ConnectionConfiguration& config);
};

class ClientMock : public ghost::Client
{
public:
	ClientMock(const ghost::ConnectionConfiguration& config);
	MOCK_METHOD0(start, bool());
	MOCK_METHOD0(stop, bool());
	MOCK_CONST_METHOD0(isRunning, bool());

	void pushMessage(const google::protobuf::Any& message);
	bool getMessage(google::protobuf::Any& message, const std::chrono::milliseconds& timeout);
};

class NotRunningClientMock : public ClientMock
{
public:
	NotRunningClientMock(const ghost::ConnectionConfiguration& config);
};

class PublisherMock : public ghost::Publisher
{
public:
	PublisherMock(const ghost::ConnectionConfiguration& config);

	MOCK_METHOD0(start, bool());
	MOCK_METHOD0(stop, bool());
	MOCK_CONST_METHOD0(isRunning, bool());
};

class SubscriberMock : public ghost::Subscriber
{
public:
	SubscriberMock(const ghost::ConnectionConfiguration& config);

	MOCK_METHOD0(start, bool());
	MOCK_METHOD0(stop, bool());
	MOCK_CONST_METHOD0(isRunning, bool());
};

class ClientHandlerMock : public ghost::ClientHandler
{
public:
	MOCK_METHOD1(configureClient, void(const std::shared_ptr<ghost::Client>& client));
	MOCK_METHOD2(handle, bool(std::shared_ptr<ghost::Client> client, bool& keepClientAlive));
};

class ConnectionManagerMock : public ghost::ConnectionManager
{
public:
	MOCK_METHOD1(createServer, std::shared_ptr<ghost::Server>(const ghost::ConnectionConfiguration& config));
	MOCK_METHOD1(createClient, std::shared_ptr<ghost::Client>(const ghost::ConnectionConfiguration& config));
	MOCK_METHOD1(createPublisher, std::shared_ptr<ghost::Publisher>(const ghost::ConnectionConfiguration& config));
	MOCK_METHOD1(createSubscriber,
		     std::shared_ptr<ghost::Subscriber>(const ghost::ConnectionConfiguration& config));
	MOCK_METHOD0(getConnectionFactory, std::shared_ptr<ghost::ConnectionFactory>());
};

class MessageMock : public ghost::Message
{
public:
	MessageMock();
	MessageMock(const MessageMock& other);
	MOCK_CONST_METHOD0(getMessageFormatName, std::string());
	MOCK_CONST_METHOD0(getMessageTypeName, std::string());
	MOCK_CONST_METHOD1(serialize, bool(std::string& result));
	MOCK_METHOD1(deserialize, bool(const std::string& payload));
};

/**
 *	Test extension for test classes that use ghost::Message mocked instances.
 */
class GhostMessageTester
{
public:
	static void setGhostMessageExpectations(MessageMock* message, const std::string& type,
						const std::string& serialized);

	static const std::string TEST_GHOST_MESSAGE_CUSTOM_FORMAT;
	static const std::string TEST_GHOST_MESSAGE_CUSTOM_TYPE_NAME;
	static const std::string TEST_GHOST_MESSAGE_CUSTOM_OTHER_TYPE_NAME;
	static const std::string TEST_GHOST_MESSAGE_CUSTOM_SERIALIZED;
	static const std::string TEST_GHOST_MESSAGE_CUSTOM_OTHER_SERIALIZED;

protected:
	void setupGhostMessages();

	std::shared_ptr<MessageMock> _ghostMessage;
	std::shared_ptr<MessageMock> _ghostMessage2;
	std::shared_ptr<MessageMock> _otherTypeGhostMessage;
	std::shared_ptr<MessageMock> _emptyGhostMessage;
};

/**
 *	Utility class to access the internal reader sink from the ghost::ReadableConnection.
 */
class ReadableConnectionWithSink : public ghost::ReadableConnection
{
public:
	ReadableConnectionWithSink(const ghost::ConnectionConfiguration& config) : ghost::ReadableConnection(config)
	{
	}

	std::shared_ptr<ghost::ReaderSink> getSink() const
	{
		return getReaderSink();
	}
};

/**
 *	Utility class to access the internal writer sink from the ghost::WritableConnection.
 */
class WritableConnectionWithSink : public ghost::WritableConnection
{
public:
	WritableConnectionWithSink(const ghost::ConnectionConfiguration& config) : ghost::WritableConnection(config)
	{
	}

	std::shared_ptr<ghost::WriterSink> getSink() const
	{
		return getWriterSink();
	}
};

#endif // GHOST_TESTS_CONNECTIONTESTUTILS_HPP
