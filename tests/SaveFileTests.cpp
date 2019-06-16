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
};

TEST_F(SaveFileTest, test_save_file)
{
	SaveFile file("test.dat");

	bool resultOpen = file.open(SaveFile::WRITE);
	ASSERT_TRUE(resultOpen);

	std::list<std::shared_ptr<ghost::internal::SaveData>> testData = generateTestdata(50,50);

	bool writeSuccess = file.write(testData);
	ASSERT_TRUE(writeSuccess);

	bool closeSuccess = file.close();
	//ASSERT_TRUE(closeSuccess);

	SaveFile file2("test.dat");

	bool resultOpen2 = file2.open(SaveFile::READ);
	ASSERT_TRUE(resultOpen2);

	std::list<std::shared_ptr<ghost::internal::SaveData>> testData2;
	bool readResult = file2.read(testData2);
	ASSERT_TRUE(readResult);

	compareTestData(testData, testData2);

	SaveFile fileAlreadyExisting("test.dat");
	bool resultOpen3 = fileAlreadyExisting.open(SaveFile::WRITE, false); // do not overwrite it, hence fails
	ASSERT_TRUE(!resultOpen3);
}
