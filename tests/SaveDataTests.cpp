#include <catch.hpp>
#include <iostream>

#include <ghost/persistence/internal/SaveFile.hpp>
#include "protobuf/tests.pb.h"

#include "PersistenceTestHelpers.hpp"

TEST_CASE("test_saveData_get_size_update")
{
	auto testData = generateTestdata(2, 5);

	auto data1 = testData.front();
	REQUIRE(data1->size() == 5);
	
	ghost::internal::protobuf::TestMessage1 msg;
	bool getSuccess = data1->get(msg, 0);
	REQUIRE(getSuccess);
	REQUIRE(msg.field1() == "field100");

	msg.set_field1("helloworld");
	bool updateSuccess = data1->replace(msg, 0);
	REQUIRE(updateSuccess);
	REQUIRE(data1->size() == 5);

	ghost::internal::protobuf::TestMessage1 msg2;
	bool getSuccess2 = data1->get(msg2, 0);
	REQUIRE(getSuccess2);
	REQUIRE(msg.field1() == msg2.field1());
}

TEST_CASE("test_saveData_put")
{
	auto testData = generateTestdata(2, 5);

	auto data1 = testData.front();
	REQUIRE(data1->size() == 5);

	// put a message
	ghost::internal::protobuf::TestMessage1 msg = ghost::internal::protobuf::TestMessage1::default_instance();
	msg.set_field1("helloworld");
	data1->put(msg);
	REQUIRE(data1->size() == 6);

	// get it back and compare
	ghost::internal::protobuf::TestMessage1 msg2;
	bool getSuccess2 = data1->get(msg2, 5);
	REQUIRE(getSuccess2);
	REQUIRE(msg.field1() == msg2.field1());
}

TEST_CASE("test_saveData_wrong_type_get")
{
	auto testData = generateTestdata(1, 0);
	auto data1 = testData.front();

	ghost::internal::protobuf::TestMessage1 msg = ghost::internal::protobuf::TestMessage1::default_instance();
	msg.set_field1("helloworld");
	data1->put(msg);
	REQUIRE(data1->size() == 1);

	ghost::internal::protobuf::TestMessage2 msg2;
	bool getSuccess = data1->get(msg2, 0);
	REQUIRE(!getSuccess);
}

TEST_CASE("test_saveData_replace_with_different_type")
{
	auto testData = generateTestdata(1, 1);
	auto data1 = testData.front();

	ghost::internal::protobuf::TestMessage1 msg;
	bool getSuccess = data1->get(msg, 0);
	REQUIRE(getSuccess);
	REQUIRE(msg.field1() == "field100");

	ghost::internal::protobuf::TestMessage2 msg2 = ghost::internal::protobuf::TestMessage2::default_instance();
	msg2.set_field1("helloworld");
	bool updateSuccess = data1->replace(msg2, 0);
	REQUIRE(updateSuccess);

	ghost::internal::protobuf::TestMessage2 msg3;
	bool getSuccess2 = data1->get(msg3, 0);
	REQUIRE(getSuccess2);
	REQUIRE(msg2.field1() == msg3.field1());
}
