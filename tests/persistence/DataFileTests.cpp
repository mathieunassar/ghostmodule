/*
 * Copyright 2020 Mathieu Nassar
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

#include "../../src/persistence/DataFile.hpp"
#include "../protobuf/tests.pb.h"
#include "PersistenceTestHelpers.hpp"

using namespace ghost::internal;

class DataFileTest : public testing::Test
{
protected:
	void SetUp() override
	{
	}

	void TearDown() override
	{
	}

	static const std::string TEST_FILE_NAME;
};

const std::string DataFileTest::TEST_FILE_NAME = "file1.dat";

TEST_F(DataFileTest, test_DataFile_open_WhenOk)
{
	DataFile file(TEST_FILE_NAME);

	bool resultOpen = file.open(DataFile::WRITE);
	ASSERT_TRUE(resultOpen);
	// close is done by the destructor
}

TEST_F(DataFileTest, test_DataFile_open_When_fileExistsButShouldNotOverwrite)
{
	DataFile file(TEST_FILE_NAME);

	bool resultOpen = file.open(DataFile::WRITE);
	ASSERT_TRUE(resultOpen);
	bool resultClose = file.close();
	ASSERT_TRUE(resultClose);

	DataFile file2(TEST_FILE_NAME);

	bool resultOpen2 = file2.open(DataFile::WRITE, false);
	ASSERT_FALSE(resultOpen2);
}

TEST_F(DataFileTest, test_DataFile_open_When_fileExistsAndShouldOverwrite)
{
	DataFile file(TEST_FILE_NAME);

	bool resultOpen = file.open(DataFile::WRITE);
	ASSERT_TRUE(resultOpen);
	file.close();

	DataFile file2(TEST_FILE_NAME);

	bool resultOpen2 = file2.open(DataFile::WRITE, true);
	ASSERT_TRUE(resultOpen2);
}

TEST_F(DataFileTest, test_DataFile_writeread_When_ok)
{
	DataFile file(TEST_FILE_NAME);

	bool resultOpen = file.open(DataFile::WRITE);
	ASSERT_TRUE(resultOpen);

	std::list<std::shared_ptr<ghost::internal::DataCollectionFile>> testData = generateTestdata(50, 50);

	bool writeSuccess = file.write(testData);
	ASSERT_TRUE(writeSuccess);

	file.close();

	DataFile file2(TEST_FILE_NAME);

	bool resultOpen2 = file2.open(DataFile::READ);
	ASSERT_TRUE(resultOpen2);

	std::list<std::shared_ptr<ghost::internal::DataCollectionFile>> testData2;
	bool readResult = file2.read(testData2);
	ASSERT_TRUE(readResult);

	compareTestData(testData, testData2);
}

TEST_F(DataFileTest, test_AnyFields)
{
	auto msg1 = ghost::internal::protobuf::TestMessage1::default_instance();
	google::protobuf::Any any;
	any.PackFrom(msg1);
	int fieldCount = any.GetDescriptor()->field_count();
	for (int i = 0; i < fieldCount; ++i)
	{
		const google::protobuf::FieldDescriptor* field = any.GetDescriptor()->field(i);
		std::cout << "field name: " << field->name() << std::endl;
	}
}