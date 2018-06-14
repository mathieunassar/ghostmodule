#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <iostream>

#include <ProtobufMessage.hpp>
#include <internal/GenericMessageConverter.hpp>
#include "../protobuf/Ghost.pb.h"

using namespace ghost;

class MyMessage : public Message
{
public:
	MyMessage(const std::string& msg) : _willfail(false), _string(msg) {}

	void setWillFail(bool val) { _willfail = val; }

	std::string getMessageFormatName() const override { return "TESTFORMAT"; }

	std::string getMessageTypeName() const override { return "MYMESSAGE"; }

	bool serialize(std::string& result) const override { result = _string; return !_willfail; }

	bool deserialize(const std::string& payload) override { _string = payload; return !_willfail; }

	bool _willfail;
	std::string _string;
};

TEST_CASE("test_conversion")
{
	auto proto = std::make_shared<ghost::internal::protobuf::GenericMessage>();
	proto->set_format("bonjour");
	proto->set_serial("ceci est mon message");
	proto->mutable_header()->set_timestamp(582);

	// Test: ProtobufMessage return values
	ProtobufMessage message(proto);
	REQUIRE(message.getMessageTypeName() == proto->GetTypeName());
	REQUIRE(message.getMessageFormatName() == internal::GHOSTMESSAGE_FORMAT_NAME);
	
	// Test: protobuf serialization from message
	std::string serialized;
	bool success = message.serialize(serialized);
	REQUIRE(success);
	REQUIRE(serialized == message.getProtobufMessage()->SerializeAsString());

	// Test: protobuf deserialization to message
	ProtobufMessage reformed = ProtobufMessage::create<ghost::internal::protobuf::GenericMessage>();
	bool success2 = reformed.deserialize(serialized);
	REQUIRE(success2);
	REQUIRE(reformed.getMessageTypeName() == proto->GetTypeName());
	REQUIRE(reformed.getMessageFormatName() == internal::GHOSTMESSAGE_FORMAT_NAME);
	auto reformedProto = std::static_pointer_cast<ghost::internal::protobuf::GenericMessage>(reformed.getProtobufMessage());
	REQUIRE(reformedProto->header().timestamp() == 582);

	// Test: conversion to Any from real protobuf message
	auto proto2 = std::make_shared<ghost::internal::protobuf::GenericMessageHeader>();
	proto2->set_timestamp(582);
	ProtobufMessage message3(proto2);

	google::protobuf::Any any;
	bool success3 = internal::GenericMessageConverter::create(any, message3);
	REQUIRE(success3);
	REQUIRE(internal::GenericMessageConverter::getTrueTypeName(any) == message3.getMessageTypeName());

	// Test: conversion from nullptr protobuf message
	google::protobuf::Any poorAny;
	ProtobufMessage nullMessage(nullptr);
	bool success4 = internal::GenericMessageConverter::create(poorAny, nullMessage);
	REQUIRE(!success4);

	// Test: conversion from any to nullptr ProtobufMessage
	bool success5 = internal::GenericMessageConverter::parse(any, nullMessage);
	REQUIRE(!success5);

	// Test: conversion from no any to real message
	ProtobufMessage message2 = ProtobufMessage::create<ghost::internal::protobuf::GenericMessage>();
	bool success6 = internal::GenericMessageConverter::parse(poorAny, message2);
	std::cout << "any type: " << internal::GenericMessageConverter::getTrueTypeName(poorAny) << std::endl;
	REQUIRE(!success6);

	// Test: conversion from any to different message type
	bool success7 = internal::GenericMessageConverter::parse(any, message2);
	REQUIRE(!success7);

	// Test: conversion from good any to good ProtobufMessage
	bool success8 = internal::GenericMessageConverter::parse(any, message3);
	REQUIRE(success8);
	auto reformedProto2 = std::static_pointer_cast<ghost::internal::protobuf::GenericMessageHeader>(message3.getProtobufMessage());
	REQUIRE(reformedProto2->timestamp() == 582);

	// Test: MyMessage to any
	MyMessage message4("superstring123");
	google::protobuf::Any customAny;
	bool success9 = internal::GenericMessageConverter::create(customAny, message4);
	REQUIRE(success9);
	REQUIRE(internal::GenericMessageConverter::getTrueTypeName(customAny) == internal::protobuf::GenericMessage().GetTypeName());

	// Test: Any to MyMessage
	MyMessage reformed2("");
	bool success10 = internal::GenericMessageConverter::parse(customAny, reformed2);
	REQUIRE(success10);
	REQUIRE(reformed2._string == "superstring123");

	// Test: Any to protobufMessage
	ProtobufMessage illformed = ProtobufMessage::create<ghost::internal::protobuf::GenericMessage>();
	bool success11 = internal::GenericMessageConverter::parse(customAny, illformed);
	REQUIRE(!success11);

	// Test: protobuf any to MyMessage
	bool success12 = internal::GenericMessageConverter::parse(any, reformed2);
	REQUIRE(!success12);

	// Test: failing mymessage to any
	message4.setWillFail(true);
	google::protobuf::Any customAny2;
	bool success13 = internal::GenericMessageConverter::create(customAny2, message4);
	REQUIRE(!success13);

	// Test any to failing mymessage
	bool success14 = internal::GenericMessageConverter::parse(customAny, message4);
	REQUIRE(!success14);
}
