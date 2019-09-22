/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless ASSERT_TRUEd by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <gtest/gtest.h>
#include <google/protobuf/wrappers.pb.h>

#include <ghost/connection/internal/ProtobufMessage.hpp>
#include <ghost/connection/internal/GenericMessageConverter.hpp>
#include "ConnectionTestUtils.hpp"

using testing::_;

/**
 *	This test class groups the following test categories:
 *	- GenericParser + ghost::Message + ghost::ProtobufMessage
 */
class MessageTests : public testing::Test, public GhostMessageTester
{
protected:
	void SetUp() override
	{
		setupProtobufMessages();
		setupGhostMessages();
	}
	
	void TearDown() override
	{
	
	}

	void setupProtobufMessages()
	{
		_doubleValue = std::make_shared<google::protobuf::DoubleValue>();
		_doubleValue->set_value(TEST_DOUBLE_VALUE);
		_message = std::make_shared<ghost::internal::ProtobufMessage>(_doubleValue);
		_message2 = std::make_shared<ghost::internal::ProtobufMessage>(_doubleValue);
		_otherTypeMessage = std::make_shared<ghost::internal::ProtobufMessage>(std::make_shared<google::protobuf::Int32Value>());
		_emptyMessage = std::make_shared<ghost::internal::ProtobufMessage>(nullptr);
	}

	std::shared_ptr<ghost::internal::ProtobufMessage> _message;
	std::shared_ptr<ghost::internal::ProtobufMessage> _message2;
	std::shared_ptr<ghost::internal::ProtobufMessage> _otherTypeMessage;
	std::shared_ptr<ghost::internal::ProtobufMessage> _emptyMessage;

	std::shared_ptr<google::protobuf::DoubleValue> _doubleValue;
	google::protobuf::Any _any;

	static const std::string TEST_BAD_SERIALIZED_MESSAGE;
	static const double TEST_DOUBLE_VALUE;
};

const std::string MessageTests::TEST_BAD_SERIALIZED_MESSAGE = "Bad message";
const double MessageTests::TEST_DOUBLE_VALUE = 42;

/* ProtobufMessage class */

TEST_F(MessageTests, test_ProtobufMessage_typeInformationIsCorrect_When_defaultConstructed)
{
	ASSERT_TRUE(_message->getMessageTypeName() == _doubleValue->descriptor()->full_name());
	ASSERT_TRUE(_message->getMessageFormatName() == ghost::internal::GHOSTMESSAGE_FORMAT_NAME);
}

TEST_F(MessageTests, test_ProtobufMessage_serializationSucceeds_When_defaultConstructed)
{
	std::string serialized;
	bool serializationResult = _message->serialize(serialized);
	
	ASSERT_TRUE(serializationResult);
	ASSERT_TRUE(serialized == _doubleValue->SerializeAsString());
}

TEST_F(MessageTests, test_ProtobufMessage_serializationFails_When_emptyConstructed)
{
	ghost::internal::ProtobufMessage message({});
	std::string serialized;
	bool serializationResult = message.serialize(serialized);

	ASSERT_FALSE(serializationResult);
}

TEST_F(MessageTests, test_ProtobufMessage_deserializationSucceeds_When_previouslySerializedMessageIsGiven)
{
	std::string serialized;
	_message->serialize(serialized);
	bool deserializationResult = _message2->deserialize(serialized);

	ASSERT_TRUE(deserializationResult);
	ASSERT_TRUE(_message2->getMessageTypeName() == _doubleValue->descriptor()->full_name());
	ASSERT_TRUE(_message2->getMessageFormatName() == ghost::internal::GHOSTMESSAGE_FORMAT_NAME);
	auto proto = std::static_pointer_cast<google::protobuf::DoubleValue>(_message2->getProtobufMessage());
	ASSERT_TRUE(proto->value() == TEST_DOUBLE_VALUE);
}

TEST_F(MessageTests, test_ProtobufMessage_deserializationFails_When_badMessageIsGiven)
{
	bool deserializationResult = _message->deserialize(TEST_BAD_SERIALIZED_MESSAGE);
	ASSERT_FALSE(deserializationResult);
}

/* GenericMessageConverter */

TEST_F(MessageTests, test_GenericMessageConverter_createFromProtobufSucceeds_When_protobufMessageIsGiven)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_message);
	ASSERT_TRUE(creationSuccess);
	ASSERT_TRUE(ghost::internal::GenericMessageConverter::getTrueTypeName(_any) == _message->getMessageTypeName());
}

TEST_F(MessageTests, test_GenericMessageConverter_createFromProtobufFailsWithoutCrashing_When_nullptrIsGiven)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_emptyMessage);
	ASSERT_FALSE(creationSuccess);
}

TEST_F(MessageTests, test_GenericMessageConverter_parseToProtobufSucceeds_When_validAnyMessageIsGiven)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_message);
	ASSERT_TRUE(creationSuccess);
	bool parsingSuccess = ghost::internal::GenericMessageConverter::parse(_any, *_message2);
	ASSERT_TRUE(parsingSuccess);
	ASSERT_TRUE(_message2->getMessageFormatName() == _message->getMessageFormatName());
	ASSERT_TRUE(_message2->getMessageTypeName() == _message->getMessageTypeName());
}

TEST_F(MessageTests, test_GenericMessageConverter_parseToProtobufFails_When_emptyAnyIsGiven)
{
	bool parsingSuccess = ghost::internal::GenericMessageConverter::parse(_any, *_message);
	ASSERT_FALSE(parsingSuccess);
}

TEST_F(MessageTests, test_GenericMessageConverter_parseToProtobufFails_When_differentAnyMessageTypeIsGiven)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_message);
	ASSERT_TRUE(creationSuccess);
	bool parsingSuccess = ghost::internal::GenericMessageConverter::parse(_any, *_otherTypeMessage);
	ASSERT_FALSE(parsingSuccess);
}

TEST_F(MessageTests, test_GenericMessageConverter_createFromGhostMessageSucceeds_When_customGhostMessageIsGiven)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_ghostMessage);

	ASSERT_TRUE(creationSuccess);
	ASSERT_TRUE(ghost::internal::GenericMessageConverter::getFormatName(_any) == _ghostMessage->getMessageFormatName());
	ASSERT_TRUE(ghost::internal::GenericMessageConverter::getFormatAndName(_any).first == _ghostMessage->getMessageFormatName());
	ASSERT_TRUE(ghost::internal::GenericMessageConverter::getFormatAndName(_any).second == _ghostMessage->getMessageTypeName());
}

TEST_F(MessageTests, test_GenericMessageConverter_parseToGhostMessageSucceeds_When_validAnyMessageIsGiven)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_ghostMessage);
	ASSERT_TRUE(creationSuccess);

	bool parsingSuccess = ghost::internal::GenericMessageConverter::parse(_any, *_ghostMessage2);
	ASSERT_TRUE(parsingSuccess);
	ASSERT_TRUE(_ghostMessage2->getMessageFormatName() == _ghostMessage->getMessageFormatName());
	ASSERT_TRUE(_ghostMessage2->getMessageTypeName() == _ghostMessage->getMessageTypeName());
}

TEST_F(MessageTests, test_GenericMessageConverter_parseToGhostMessageFails_When_differentAnyMessageTypeIsGiven)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_ghostMessage);
	ASSERT_TRUE(creationSuccess);
	bool parsingSuccess = ghost::internal::GenericMessageConverter::parse(_any, *_otherTypeGhostMessage);
	ASSERT_FALSE(parsingSuccess);
}

TEST_F(MessageTests, test_GenericMessageConverter_parseToProtobufFails_When_anyFromGhostMessageIsGiven)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_ghostMessage);
	ASSERT_TRUE(creationSuccess);
	bool parsingSuccess = ghost::internal::GenericMessageConverter::parse(_any, *_message);
	ASSERT_FALSE(parsingSuccess);
}

TEST_F(MessageTests, test_GenericMessageConverter_parseToGhostMessageFails_When_anyFromProtobufIsGiven)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_message);
	ASSERT_TRUE(creationSuccess);
	bool parsingSuccess = ghost::internal::GenericMessageConverter::parse(_any, *_ghostMessage2);
	ASSERT_FALSE(parsingSuccess);
}

TEST_F(MessageTests, test_GenericMessageConverter_createFromGhostMessageFails_When_serializationFails)
{
	EXPECT_CALL(*_ghostMessage, serialize(_)).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(false));

	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_ghostMessage);
	ASSERT_FALSE(creationSuccess);
}

TEST_F(MessageTests, test_GenericMessageConverter_parseToGhostMessageFails_When_deserializationFails)
{
	bool creationSuccess = ghost::internal::GenericMessageConverter::create(_any, *_ghostMessage);
	ASSERT_TRUE(creationSuccess);

	EXPECT_CALL(*_ghostMessage2, deserialize(_)).Times(testing::AnyNumber()).WillRepeatedly(testing::Return(false));

	bool parsingSuccess = ghost::internal::GenericMessageConverter::parse(_any, *_ghostMessage2);
	ASSERT_FALSE(parsingSuccess);
}
