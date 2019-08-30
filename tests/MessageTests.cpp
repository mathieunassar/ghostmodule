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
#include <ghost/connection/internal/ProtobufMessage.hpp>

/**
 *	This test class groups the following test categories:
 *	- GenericParser + ghost::Message + ghost::ProtobufMessage
 */
class MessageTests : public testing::Test
{
protected:
	void SetUp() override
	{
	
	}
	
	void TearDown() override
	{
	
	}
};

/* ProtobufMessage class */

TEST_F(MessageTests, test_ProtobufMessage_typeInformationIsCorrect_When_defaultConstructed)
{
	
}

TEST_F(MessageTests, test_ProtobufMessage_serializationSucceeds_When_defaultConstructed)
{

}

TEST_F(MessageTests, test_ProtobufMessage_serializationFails_When_emptyConstructed)
{

}

TEST_F(MessageTests, test_ProtobufMessage_deserializationSucceeds_When_previouslySerializedMessageIsGiven)
{

}

TEST_F(MessageTests, test_ProtobufMessage_deserializationFails_When_badMessageIsGiven)
{

}

/* GenericMessageConverter */

TEST_F(MessageTests, test_GenericMessageConverter_createFromProtobufSucceeds_When_protobufMessageIsGiven)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_createFromProtobufFailsWithoutCrashing_When_nullptrIsGiven)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_parseToProtobufSucceeds_When_validAnyMessageIsGiven)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_parseToProtobufFails_When_emptyAnyIsGiven)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_parseToProtobufFails_When_differentAnyMessageTypeIsGiven)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_createFromGhostMessageSucceeds_When_customGhostMessageIsGiven)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_parseToGhostMessageSucceeds_When_validAnyMessageIsGiven)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_parseToProtobufFails_When_anyFromGhostMessageIsGiven)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_parseToGhostMessageFails_When_anyFromProtobufIsGiven)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_createFromGhostMessageFails_When_serializationFails)
{

}

TEST_F(MessageTests, test_GenericMessageConverter_parseToGhostMessageFails_When_deserializationFails)
{

}

//class MyMessage : public Message
//{
//public:
//	MyMessage(const std::string& msg) : _willfail(false), _string(msg) {}
//
//	void setWillFail(bool val) { _willfail = val; }
//
//	std::string getMessageFormatName() const override { return "TESTFORMAT"; }
//
//	std::string getMessageTypeName() const override { return "MYMESSAGE"; }
//
//	bool serialize(std::string& result) const override { result = _string; return !_willfail; }
//
//	bool deserialize(const std::string& payload) override { _string = payload; return !_willfail; }
//
//	bool _willfail;
//	std::string _string;
//};
//
//class MessageHandlerMock : public internal::MessageHandler
//{
//public:
//	internal::MessageHandler* getInternal()
//	{
//		return _internal;
//	}
//};
//
//TEST_F(MessageTests, test_conversion)
//{
//	auto proto = std::make_shared<ghost::internal::protobuf::GenericMessage>();
//	proto->set_format("bonjour");
//	proto->set_serial("ceci est mon message");
//	proto->mutable_header()->set_timestamp(582);
//
//	// Test: ProtobufMessage return values
//	ProtobufMessage message(proto);
//	ASSERT_TRUE(message.getMessageTypeName() == proto->GetTypeName());
//	ASSERT_TRUE(message.getMessageFormatName() == internal::GHOSTMESSAGE_FORMAT_NAME);
//	
//	// Test: protobuf serialization from message
//	std::string serialized;
//	bool success = message.serialize(serialized);
//	ASSERT_TRUE(success);
//	ASSERT_TRUE(serialized == message.getProtobufMessage()->SerializeAsString());
//
//	// Test: protobuf deserialization to message
//	ProtobufMessage reformed = ProtobufMessage::create<ghost::internal::protobuf::GenericMessage>();
//	bool success2 = reformed.deserialize(serialized);
//	ASSERT_TRUE(success2);
//	ASSERT_TRUE(reformed.getMessageTypeName() == proto->GetTypeName());
//	ASSERT_TRUE(reformed.getMessageFormatName() == internal::GHOSTMESSAGE_FORMAT_NAME);
//	auto reformedProto = std::static_pointer_cast<ghost::internal::protobuf::GenericMessage>(reformed.getProtobufMessage());
//	ASSERT_TRUE(reformedProto->header().timestamp() == 582);
//
//	// Test: conversion to Any from real protobuf message
//	auto proto2 = std::make_shared<ghost::internal::protobuf::GenericMessageHeader>();
//	proto2->set_timestamp(582);
//	ProtobufMessage message3(proto2);
//
//	google::protobuf::Any any;
//	bool success3 = internal::GenericMessageConverter::create(any, message3);
//	ASSERT_TRUE(success3);
//	ASSERT_TRUE(internal::GenericMessageConverter::getTrueTypeName(any) == message3.getMessageTypeName());
//
//	// Test: conversion from nullptr protobuf message
//	google::protobuf::Any poorAny;
//	ProtobufMessage nullMessage(nullptr);
//	bool success4 = internal::GenericMessageConverter::create(poorAny, nullMessage);
//	ASSERT_TRUE(!success4);
//
//	// Test: conversion from any to nullptr ProtobufMessage
//	bool success5 = internal::GenericMessageConverter::parse(any, nullMessage);
//	ASSERT_TRUE(!success5);
//
//	// Test: conversion from no any to real message
//	ProtobufMessage message2 = ProtobufMessage::create<ghost::internal::protobuf::GenericMessage>();
//	bool success6 = internal::GenericMessageConverter::parse(poorAny, message2);
//	std::cout << "any type: " << internal::GenericMessageConverter::getTrueTypeName(poorAny) << std::endl;
//	ASSERT_TRUE(!success6);
//
//	// Test: conversion from any to different message type
//	bool success7 = internal::GenericMessageConverter::parse(any, message2);
//	ASSERT_TRUE(!success7);
//
//	// Test: conversion from good any to good ProtobufMessage
//	bool success8 = internal::GenericMessageConverter::parse(any, message3);
//	ASSERT_TRUE(success8);
//	auto reformedProto2 = std::static_pointer_cast<ghost::internal::protobuf::GenericMessageHeader>(message3.getProtobufMessage());
//	ASSERT_TRUE(reformedProto2->timestamp() == 582);
//
//	// Test: MyMessage to any
//	MyMessage message4("superstring123");
//	google::protobuf::Any customAny;
//	bool success9 = internal::GenericMessageConverter::create(customAny, message4);
//	ASSERT_TRUE(success9);
//	ASSERT_TRUE(internal::GenericMessageConverter::getTrueTypeName(customAny) == internal::protobuf::GenericMessage().GetTypeName());
//
//	// Test: Any to MyMessage
//	MyMessage reformed2("");
//	bool success10 = internal::GenericMessageConverter::parse(customAny, reformed2);
//	ASSERT_TRUE(success10);
//	ASSERT_TRUE(reformed2._string == "superstring123");
//
//	// Test: Any to protobufMessage
//	ProtobufMessage illformed = ProtobufMessage::create<ghost::internal::protobuf::GenericMessage>();
//	bool success11 = internal::GenericMessageConverter::parse(customAny, illformed);
//	ASSERT_TRUE(!success11);
//
//	// Test: protobuf any to MyMessage
//	bool success12 = internal::GenericMessageConverter::parse(any, reformed2);
//	ASSERT_TRUE(!success12);
//
//	// Test: failing mymessage to any
//	message4.setWillFail(true);
//	google::protobuf::Any customAny2;
//	bool success13 = internal::GenericMessageConverter::create(customAny2, message4);
//	ASSERT_TRUE(!success13);
//
//	// Test any to failing mymessage
//	bool success14 = internal::GenericMessageConverter::parse(customAny, message4);
//	ASSERT_TRUE(!success14);
//}
