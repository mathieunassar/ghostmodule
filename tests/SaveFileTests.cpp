#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <iostream>

#include <ghost/persistence/internal/SaveFile.hpp>
#include "protobuf/tests.pb.h"

#include "PersistenceTestHelpers.hpp"

using namespace ghost::internal;

TEST_CASE("test_save_file")
{
	SaveFile file("test.dat");

	bool resultOpen = file.open(SaveFile::WRITE);
	REQUIRE(resultOpen);

	std::list<std::shared_ptr<ghost::SaveData>> testData = generateTestdata(50,50);

	bool writeSuccess = file.write(testData);
	REQUIRE(writeSuccess);

	bool closeSuccess = file.close();
	//REQUIRE(closeSuccess);

	SaveFile file2("test.dat");

	bool resultOpen2 = file2.open(SaveFile::READ);
	REQUIRE(resultOpen2);

	std::list<std::shared_ptr<ghost::SaveData>> testData2;
	bool readResult = file2.read(testData2);
	REQUIRE(readResult);

	compareTestData(testData, testData2);
}
