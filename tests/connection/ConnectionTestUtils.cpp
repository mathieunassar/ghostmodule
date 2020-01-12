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

#include "ConnectionTestUtils.hpp"

using testing::_;

const std::string GhostMessageTester::TEST_GHOST_MESSAGE_CUSTOM_FORMAT = "Format";
const std::string GhostMessageTester::TEST_GHOST_MESSAGE_CUSTOM_TYPE_NAME = "Type name";
const std::string GhostMessageTester::TEST_GHOST_MESSAGE_CUSTOM_OTHER_TYPE_NAME = "Other type name";
const std::string GhostMessageTester::TEST_GHOST_MESSAGE_CUSTOM_SERIALIZED = "Serialized";
const std::string GhostMessageTester::TEST_GHOST_MESSAGE_CUSTOM_OTHER_SERIALIZED = "Other serialized";

ServerMock::ServerMock(const ghost::ConnectionConfiguration& config)
{
	EXPECT_CALL(*this, start()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*this, isRunning()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*this, stop()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
}

NotRunningServerMock::NotRunningServerMock(const ghost::ConnectionConfiguration& config) : ServerMock(config)
{
	EXPECT_CALL(*this, start()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(false));
	EXPECT_CALL(*this, isRunning()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(false));
}

ClientMock::ClientMock(const ghost::ConnectionConfiguration& config) : ghost::Client(config)
{
	EXPECT_CALL(*this, start()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*this, isRunning()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*this, stop()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
}

void ClientMock::pushMessage(const google::protobuf::Any& message)
{
	getReaderSink()->put(message);
}

NotRunningClientMock::NotRunningClientMock(const ghost::ConnectionConfiguration& config) : ClientMock(config)
{
	EXPECT_CALL(*this, start()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(false));
	EXPECT_CALL(*this, isRunning()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(false));
}

bool ClientMock::getMessage(google::protobuf::Any& message, const std::chrono::milliseconds& timeout)
{
	return getWriterSink()->get(message, timeout);
}

PublisherMock::PublisherMock(const ghost::ConnectionConfiguration& config) : ghost::Publisher(config)
{
	EXPECT_CALL(*this, isRunning()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*this, stop()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
}

SubscriberMock::SubscriberMock(const ghost::ConnectionConfiguration& config) : ghost::Subscriber(config)
{
	EXPECT_CALL(*this, isRunning()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
	EXPECT_CALL(*this, stop()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
}

void GhostMessageTester::setupGhostMessages()
{
	_ghostMessage = std::make_shared<MessageMock>();
	_ghostMessage2 = std::make_shared<MessageMock>();
	_otherTypeGhostMessage = std::make_shared<MessageMock>();
	_emptyGhostMessage = std::make_shared<MessageMock>();

	setGhostMessageExpectations(_ghostMessage.get(), TEST_GHOST_MESSAGE_CUSTOM_TYPE_NAME,
				    TEST_GHOST_MESSAGE_CUSTOM_SERIALIZED);
	setGhostMessageExpectations(_ghostMessage2.get(), TEST_GHOST_MESSAGE_CUSTOM_TYPE_NAME,
				    TEST_GHOST_MESSAGE_CUSTOM_SERIALIZED);
	setGhostMessageExpectations(_otherTypeGhostMessage.get(), TEST_GHOST_MESSAGE_CUSTOM_OTHER_TYPE_NAME,
				    TEST_GHOST_MESSAGE_CUSTOM_OTHER_SERIALIZED);
	setGhostMessageExpectations(_emptyGhostMessage.get(), TEST_GHOST_MESSAGE_CUSTOM_TYPE_NAME, "");
}

void GhostMessageTester::setGhostMessageExpectations(MessageMock* message, const std::string& type,
						     const std::string& serialized)
{
	EXPECT_CALL(*message, getMessageFormatName())
	    .Times(testing::AnyNumber())
	    .WillRepeatedly(testing::Return(TEST_GHOST_MESSAGE_CUSTOM_FORMAT));
	EXPECT_CALL(*message, getMessageTypeName()).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(type));
	EXPECT_CALL(*message, serialize(_))
	    .Times(testing::AnyNumber())
	    .WillRepeatedly(testing::DoAll(testing::SetArgReferee<0>(serialized), testing::Return(true)));
	EXPECT_CALL(*message, deserialize(_)).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(true));
}

MessageMock::MessageMock()
{
	GhostMessageTester::setGhostMessageExpectations(this, GhostMessageTester::TEST_GHOST_MESSAGE_CUSTOM_TYPE_NAME,
							GhostMessageTester::TEST_GHOST_MESSAGE_CUSTOM_SERIALIZED);
}

MessageMock::MessageMock(const MessageMock& other)
{
	GhostMessageTester::setGhostMessageExpectations(this, GhostMessageTester::TEST_GHOST_MESSAGE_CUSTOM_TYPE_NAME,
							GhostMessageTester::TEST_GHOST_MESSAGE_CUSTOM_SERIALIZED);
}
