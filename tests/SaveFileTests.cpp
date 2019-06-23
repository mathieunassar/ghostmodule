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

#include "../src/persistence/SaveFile.hpp"
#include "protobuf/tests.pb.h"

#include "PersistenceTestHelpers.hpp"

using namespace ghost::internal;

class SaveFileTest : public testing::Test
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

const std::string SaveFileTest::TEST_FILE_NAME = "file1.dat";

TEST_F(SaveFileTest, test_SaveFile_open_WhenOk)
{
	SaveFile file(TEST_FILE_NAME);

	bool resultOpen = file.open(SaveFile::WRITE);
	ASSERT_TRUE(resultOpen);
	// close is done by the destructor
}

TEST_F(SaveFileTest, test_SaveFile_open_When_fileExistsButShouldNotOverwrite)
{
	SaveFile file(TEST_FILE_NAME);

	bool resultOpen = file.open(SaveFile::WRITE);
	ASSERT_TRUE(resultOpen);
	bool resultClose = file.close();
	ASSERT_TRUE(resultClose);

	SaveFile file2(TEST_FILE_NAME);

	bool resultOpen2 = file2.open(SaveFile::WRITE, false);
	ASSERT_FALSE(resultOpen2);
}

TEST_F(SaveFileTest, test_SaveFile_open_When_fileExistsAndShouldOverwrite)
{
	SaveFile file(TEST_FILE_NAME);

	bool resultOpen = file.open(SaveFile::WRITE);
	ASSERT_TRUE(resultOpen);
	file.close();

	SaveFile file2(TEST_FILE_NAME);

	bool resultOpen2 = file2.open(SaveFile::WRITE, true);
	ASSERT_TRUE(resultOpen2);
}

TEST_F(SaveFileTest, test_SaveFile_writeread_When_ok)
{
	SaveFile file(TEST_FILE_NAME);

	bool resultOpen = file.open(SaveFile::WRITE);
	ASSERT_TRUE(resultOpen);

	std::list<std::shared_ptr<ghost::internal::SaveData>> testData = generateTestdata(50,50);

	bool writeSuccess = file.write(testData);
	ASSERT_TRUE(writeSuccess);

	file.close();

	SaveFile file2(TEST_FILE_NAME);

	bool resultOpen2 = file2.open(SaveFile::READ);
	ASSERT_TRUE(resultOpen2);

	std::list<std::shared_ptr<ghost::internal::SaveData>> testData2;
	bool readResult = file2.read(testData2);
	ASSERT_TRUE(readResult);

	compareTestData(testData, testData2);
}
