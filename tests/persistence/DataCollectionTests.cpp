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

#include <gtest/gtest.h>

#include <iostream>

#include "../../src/persistence/DataCollectionFile.hpp"
#include "../protobuf/tests.pb.h"
#include "PersistenceTestHelpers.hpp"

class DataCollectionFileTest : public testing::Test
{
protected:
	void SetUp() override
	{
	}

	void TearDown() override
	{
	}

	static const std::string TEST_DATA_NAME;
	static const std::string TEST_DATA_FIELD1;
	static const int TEST_DATA_FIELDINT;
	static const std::string EXPECTED_FIELD1_VALUE_FOR_I0J0;
	static const std::string EXPECTED_FIELD1_VALUE_FOR_I1J0;
};

const std::string DataCollectionFileTest::TEST_DATA_NAME = "Test";
const std::string DataCollectionFileTest::TEST_DATA_FIELD1 = "Field";
const int DataCollectionFileTest::TEST_DATA_FIELDINT = 42;
const std::string DataCollectionFileTest::EXPECTED_FIELD1_VALUE_FOR_I0J0 =
    "field100"; // from "generateTestData's implementation
const std::string DataCollectionFileTest::EXPECTED_FIELD1_VALUE_FOR_I1J0 =
    "field110"; // from "generateTestData's implementation

TEST_F(DataCollectionFileTest, test_dataCollectionFile_replace_When_ok)
{
	auto testData = generateTestdata(2, 5);

	auto data1 = testData.front();
	ASSERT_TRUE(data1->size() == 5);

	ghost::internal::protobuf::TestMessage1 msg;
	bool getSuccess = data1->get(msg, 0);
	ASSERT_TRUE(getSuccess);
	ASSERT_TRUE(msg.field1() == EXPECTED_FIELD1_VALUE_FOR_I0J0);

	msg.set_field1(TEST_DATA_FIELD1);
	bool updateSuccess = data1->replace(msg, 0);
	ASSERT_TRUE(updateSuccess);
	ASSERT_TRUE(data1->size() == 5);

	ghost::internal::protobuf::TestMessage1 msg2;
	bool getSuccess2 = data1->get(msg2, 0);
	ASSERT_TRUE(getSuccess2);
	ASSERT_TRUE(msg.field1() == msg2.field1());
}

TEST_F(DataCollectionFileTest, test_dataCollectionFile_putget_When_ok)
{
	auto testData = generateTestdata(2, 5);

	auto data1 = testData.front();
	ASSERT_TRUE(data1->size() == 5);

	// put a message
	ghost::internal::protobuf::TestMessage1 msg = ghost::internal::protobuf::TestMessage1::default_instance();
	msg.set_field1(TEST_DATA_FIELD1);
	data1->put(msg);
	ASSERT_TRUE(data1->size() == 6);

	// get it back and compare
	ghost::internal::protobuf::TestMessage1 msg2;
	bool getSuccess2 = data1->get(msg2, 5);
	ASSERT_TRUE(getSuccess2);
	ASSERT_TRUE(msg.field1() == msg2.field1());
}

TEST_F(DataCollectionFileTest, test_dataCollectionFile_get_When_wrongTypeProvided)
{
	auto testData = generateTestdata(1, 0);
	auto data1 = testData.front();

	ghost::internal::protobuf::TestMessage1 msg = ghost::internal::protobuf::TestMessage1::default_instance();
	msg.set_field1(TEST_DATA_FIELD1);
	data1->put(msg);
	ASSERT_TRUE(data1->size() == 1);

	ghost::internal::protobuf::TestMessage2 msg2;
	bool getSuccess = data1->get(msg2, 0);
	ASSERT_TRUE(!getSuccess);
}

TEST_F(DataCollectionFileTest, test_dataCollectionFile_replace_When_differentTypeProvided)
{
	auto testData = generateTestdata(1, 1);
	auto data1 = testData.front();

	ghost::internal::protobuf::TestMessage1 msg;
	bool getSuccess = data1->get(msg, 0);
	ASSERT_TRUE(getSuccess);
	ASSERT_TRUE(msg.field1() == EXPECTED_FIELD1_VALUE_FOR_I0J0);

	ghost::internal::protobuf::TestMessage2 msg2 = ghost::internal::protobuf::TestMessage2::default_instance();
	msg2.set_field4(TEST_DATA_FIELDINT);
	bool updateSuccess = data1->replace(msg2, 0);
	ASSERT_TRUE(updateSuccess);

	ghost::internal::protobuf::TestMessage2 msg3;
	bool getSuccess2 = data1->get(msg3, 0);
	ASSERT_TRUE(getSuccess2);
	ASSERT_TRUE(msg2.field4() == msg3.field4());
}

TEST_F(DataCollectionFileTest, test_dataCollectionFile_remove_When_ok)
{
	auto testData = generateTestdata(1, 2);
	auto data1 = testData.front();

	bool removeSuccess0 = data1->remove(2);
	ASSERT_TRUE(!removeSuccess0);

	bool removeSuccess = data1->remove(0);
	ASSERT_TRUE(removeSuccess);
	ASSERT_TRUE(data1->size() == 1);

	ghost::internal::protobuf::TestMessage1 msg;
	bool getSuccess = data1->get(msg, 1);
	ASSERT_TRUE(getSuccess);
	ASSERT_TRUE(msg.field1() == EXPECTED_FIELD1_VALUE_FOR_I1J0);

	bool removeSuccess2 = data1->remove(1);
	ASSERT_TRUE(removeSuccess2);
	ASSERT_TRUE(data1->size() == 0);

	bool removeSuccess3 = data1->remove(0);
	ASSERT_TRUE(!removeSuccess3);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_getIf_When_oneElementMatches)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg, msg2;
	msg.set_field1(TEST_DATA_FIELD1);
	data->put(msg);
	data->put(msg2);

	auto gotIf = data->get_if<ghost::internal::protobuf::TestMessage1>(
	    [&](const auto& dat, size_t id) { return dat.field1() == TEST_DATA_FIELD1; });
	ASSERT_EQ(gotIf.size(), 1);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_getIf_When_twoElementsMatches)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg, msg2;
	msg.set_field1(TEST_DATA_FIELD1);
	msg2.set_field1(TEST_DATA_FIELD1);
	data->put(msg);
	data->put(msg2);

	auto gotIf = data->get_if<ghost::internal::protobuf::TestMessage1>(
	    [&](const auto& dat, size_t id) { return dat.field1() == TEST_DATA_FIELD1; });
	ASSERT_EQ(gotIf.size(), 2);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_getIf_When_noElementMatches)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg, msg2;
	data->put(msg);
	data->put(msg2);

	auto gotIf = data->get_if<ghost::internal::protobuf::TestMessage1>(
	    [&](const auto& dat, size_t id) { return dat.field1() == TEST_DATA_FIELD1; });
	ASSERT_EQ(gotIf.size(), 0);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_getIf_When_dataHasAnotherType)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg;
	msg.set_field1(TEST_DATA_FIELD1);
	ghost::internal::protobuf::TestMessage2 msg2;
	data->put(msg);
	data->put(msg2);

	auto gotIf = data->get_if<ghost::internal::protobuf::TestMessage1>(
	    [&](const auto& dat, size_t id) { return dat.field1() == TEST_DATA_FIELD1; });
	ASSERT_EQ(gotIf.size(), 1);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_removeIf_When_oneElementMatches)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg, msg2;
	msg.set_field1(TEST_DATA_FIELD1);
	data->put(msg);
	data->put(msg2);

	auto removed = data->remove_if<ghost::internal::protobuf::TestMessage1>(
	    [&](const auto& dat, size_t id) { return dat.field1() == TEST_DATA_FIELD1; });
	ASSERT_EQ(removed, 1);
	ASSERT_EQ(data->size(), 1);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_removeIf_When_twoElementsMatches)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg, msg2;
	msg.set_field1(TEST_DATA_FIELD1);
	msg2.set_field1(TEST_DATA_FIELD1);
	data->put(msg);
	data->put(msg2);

	auto removed = data->remove_if<ghost::internal::protobuf::TestMessage1>(
	    [&](const auto& dat, size_t id) { return dat.field1() == TEST_DATA_FIELD1; });
	ASSERT_EQ(removed, 2);
	ASSERT_EQ(data->size(), 0);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_removeIf_When_noElementMatches)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg, msg2;
	data->put(msg);
	data->put(msg2);

	auto removed = data->remove_if<ghost::internal::protobuf::TestMessage1>(
	    [&](const auto& dat, size_t id) { return dat.field1() == TEST_DATA_FIELD1; });
	ASSERT_EQ(removed, 0);
	ASSERT_EQ(data->size(), 2);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_removeIf_When_dataHasAnotherType)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg;
	msg.set_field1(TEST_DATA_FIELD1);
	ghost::internal::protobuf::TestMessage2 msg2;
	data->put(msg);
	data->put(msg2);

	auto removed = data->remove_if<ghost::internal::protobuf::TestMessage1>(
	    [&](const auto& dat, size_t id) { return dat.field1() == TEST_DATA_FIELD1; });
	ASSERT_EQ(removed, 1);
	ASSERT_EQ(data->size(), 1);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_replaceIf_When_oneElementMatches)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg, msg2;
	msg.set_field1(TEST_DATA_FIELD1);
	data->put(msg);
	data->put(msg2);

	auto updated = data->replace_if<ghost::internal::protobuf::TestMessage1>([&](auto& dat, size_t id) {
		if (dat.field1() == TEST_DATA_FIELD1)
		{
			dat.set_field1(TEST_DATA_NAME);
			return true;
		}
		return false;
	});
	ASSERT_EQ(updated, 1);

	auto up = ghost::internal::protobuf::TestMessage1::default_instance();
	ASSERT_TRUE(data->get(up, 0));
	ASSERT_TRUE(up.field1() == TEST_DATA_NAME);
	ASSERT_TRUE(data->get(up, 1));
	ASSERT_TRUE(up.field1().empty());
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_replaceIf_When_twoElementsMatches)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg, msg2;
	msg.set_field1(TEST_DATA_FIELD1);
	msg2.set_field1(TEST_DATA_FIELD1);
	data->put(msg);
	data->put(msg2);

	auto updated = data->replace_if<ghost::internal::protobuf::TestMessage1>([&](auto& dat, size_t id) {
		if (dat.field1() == TEST_DATA_FIELD1)
		{
			dat.set_field1(TEST_DATA_NAME);
			return true;
		}
		return false;
	});
	ASSERT_EQ(updated, 2);

	auto up = ghost::internal::protobuf::TestMessage1::default_instance();
	ASSERT_TRUE(data->get(up, 0));
	ASSERT_TRUE(up.field1() == TEST_DATA_NAME);
	ASSERT_TRUE(data->get(up, 1));
	ASSERT_TRUE(up.field1() == TEST_DATA_NAME);
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_replaceIf_When_noElementMatches)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg, msg2;
	data->put(msg);
	data->put(msg2);

	auto updated = data->replace_if<ghost::internal::protobuf::TestMessage1>([&](auto& dat, size_t id) {
		if (dat.field1() == TEST_DATA_FIELD1)
		{
			dat.set_field1(TEST_DATA_NAME);
			return true;
		}
		return false;
	});
	ASSERT_EQ(updated, 0);

	auto up = ghost::internal::protobuf::TestMessage1::default_instance();
	ASSERT_TRUE(data->get(up, 0));
	ASSERT_TRUE(up.field1().empty());
	ASSERT_TRUE(data->get(up, 1));
	ASSERT_TRUE(up.field1().empty());
}

TEST_F(DataCollectionFileTest, test_DataCollectionFile_replaceIf_When_dataHasAnotherType)
{
	auto data = std::make_shared<ghost::internal::DataCollectionFile>("", 0);
	ghost::internal::protobuf::TestMessage1 msg;
	msg.set_field1(TEST_DATA_FIELD1);
	ghost::internal::protobuf::TestMessage2 msg2;
	data->put(msg);
	data->put(msg2);

	auto updated = data->replace_if<ghost::internal::protobuf::TestMessage1>([&](auto& dat, size_t id) {
		if (dat.field1() == TEST_DATA_FIELD1)
		{
			dat.set_field1(TEST_DATA_NAME);
			return true;
		}
		return false;
	});
	ASSERT_EQ(updated, 1);

	auto up = ghost::internal::protobuf::TestMessage1::default_instance();
	ASSERT_TRUE(data->get(up, 0));
	ASSERT_TRUE(up.field1() == TEST_DATA_NAME);
	ASSERT_FALSE(data->get(up, 1));
}