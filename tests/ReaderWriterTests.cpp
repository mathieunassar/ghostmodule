#include <catch.hpp>
#include <iostream>

#include <ghost/connection/internal/GenericReader.hpp>
#include <ghost/connection/internal/GenericWriter.hpp>

#include "../protobuf/Ghost.pb.h"

using namespace ghost::internal;

TEST_CASE("test_genericreader_proto")
{
	GenericReader<protobuf::GenericMessageHeader> reader(false);

	// create a test message
	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
	proto->set_timestamp(582);

	// convert it to any
	google::protobuf::Any anymessage;
	anymessage.PackFrom(*proto);

	// feed the reader
	reader.put(anymessage);

	// check that you can read it now
	protobuf::GenericMessageHeader newmessage;
	bool readSuccess = reader.read(newmessage);
	REQUIRE(readSuccess);
	REQUIRE(newmessage.timestamp() == 582);

	// read again, should return false because the call is non blocking
	bool readSuccess2 = reader.read(newmessage);
	REQUIRE(!readSuccess2);

	// add it again...
	reader.put(anymessage);

	// copy the reader
	GenericReader<protobuf::GenericMessageHeader> reader2(reader, false);

	// normally it is possible to read from it
	protobuf::GenericMessageHeader newmessage2;
	bool readSuccess3 = reader2.read(newmessage2);
	REQUIRE(readSuccess3);
	REQUIRE(newmessage2.timestamp() == 582);

	// and reading from the old one is now finished (they share the queue)
	bool readSuccess4 = reader.read(newmessage2);
	REQUIRE(!readSuccess4);
}

TEST_CASE("test_genericWriter_any")
{
	GenericWriter<google::protobuf::Any> writer(false);

	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
	proto->set_timestamp(582);
	google::protobuf::Any anymessage;
	anymessage.PackFrom(*proto);

	bool writeSuccess = writer.write(anymessage);
	REQUIRE(writeSuccess);
}

TEST_CASE("test_genericwriter_proto")
{
	GenericWriter<protobuf::GenericMessageHeader> writer(false);

	// create a test message
	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
	proto->set_timestamp(582);

	bool writeSuccess = writer.write(*proto);
	REQUIRE(writeSuccess);

	google::protobuf::Any anymessage;
	bool getSuccess = writer.get(anymessage);
	REQUIRE(getSuccess);

	protobuf::GenericMessageHeader newmessage;
	bool unpackSuccess = anymessage.UnpackTo(&newmessage);
	REQUIRE(unpackSuccess);
	REQUIRE(newmessage.timestamp() == 582);
}

TEST_CASE("test_genericreader_ghost")
{
	GenericReader<ghost::ProtobufMessage> reader(false);

	// create a test message
	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
	proto->set_timestamp(582);
	ghost::ProtobufMessage message(proto);

	// convert it to any
	google::protobuf::Any anymessage;
	anymessage.PackFrom(*proto);

	reader.put(anymessage);

	// check that it is readable
	ghost::ProtobufMessage message2(std::make_shared<protobuf::GenericMessageHeader>());
	bool readSuccess = reader.read(message2);
	REQUIRE(readSuccess);
}

TEST_CASE("test_genericwriter_ghost")
{
	GenericWriter<ghost::ProtobufMessage> writer(false);

	// create a test message
	auto proto = std::make_shared<protobuf::GenericMessageHeader>();
	proto->set_timestamp(582);
	ghost::ProtobufMessage message(proto);

	bool writeSuccess = writer.write(message);
	REQUIRE(writeSuccess);

	google::protobuf::Any anymessage;
	bool getSuccess = writer.get(anymessage);
	REQUIRE(getSuccess);
}
