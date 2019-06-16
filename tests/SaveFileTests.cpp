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

	std::list<std::shared_ptr<ghost::SaveData>> testData = generateTestdata(50,50);

	bool writeSuccess = file.write(testData);
	ASSERT_TRUE(writeSuccess);

	bool closeSuccess = file.close();
	//ASSERT_TRUE(closeSuccess);

	SaveFile file2("test.dat");

	bool resultOpen2 = file2.open(SaveFile::READ);
	ASSERT_TRUE(resultOpen2);

	std::list<std::shared_ptr<ghost::SaveData>> testData2;
	bool readResult = file2.read(testData2);
	ASSERT_TRUE(readResult);

	compareTestData(testData, testData2);

	SaveFile fileAlreadyExisting("test.dat");
	bool resultOpen3 = fileAlreadyExisting.open(SaveFile::WRITE, false); // do not overwrite it, hence fails
	ASSERT_TRUE(!resultOpen3);
}
