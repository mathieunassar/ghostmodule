#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <iostream>

#include <ghost/persistence/internal/SaveFile.hpp>
#include "protobuf/tests.pb.h"

using namespace ghost::internal;

std::list<std::shared_ptr<SaveData>> generateTestdata(size_t saveDataSize, size_t dataPerSet)
{
	std::list<std::shared_ptr<SaveData>> testData;
	for (int j = 0; j < saveDataSize; j++)
	{
		std::vector<std::shared_ptr<google::protobuf::Any>> data;
		for (int i = 0; i < dataPerSet; i++)
		{
			std::string field1Value = "field1" + std::to_string(i) + std::to_string(j);
			auto msg = ghost::internal::protobuf::TestMessage1::default_instance();
			msg.set_field1(field1Value);
			auto any = std::make_shared<google::protobuf::Any>();
			any->PackFrom(msg);
			data.push_back(any);
		}
		std::string saveDataName = "super" + std::to_string(j);
		auto savedata = std::make_shared<SaveData>(saveDataName, data);
		
		testData.push_back(savedata);
	}
	return testData;
}

void compareTestData(const std::list<std::shared_ptr<SaveData>>& data1,
	const std::list<std::shared_ptr<SaveData>>& data2)
{
	auto it = data1.begin();
	auto it2 = data2.begin();
	while (it != data1.end())
	{
		REQUIRE(it2 != data2.end());
		REQUIRE((*it)->getName() == (*it2)->getName());
		auto d1 = (*it)->getData();
		auto d2 = (*it2)->getData();

		REQUIRE(d1.size() == d2.size());
		for (size_t i = 0; i < d1.size(); i++)
		{
			const auto& elem = d1[i];
			const auto& elem2 = d2[i];

			auto msg1 = ghost::internal::protobuf::TestMessage1::default_instance();
			auto msg2 = ghost::internal::protobuf::TestMessage1::default_instance();
			bool unpack1 = elem->UnpackTo(&msg1);
			bool unpack2 = elem2->UnpackTo(&msg2);
			REQUIRE(unpack1);
			REQUIRE(unpack2);
			REQUIRE(msg1.field1() == msg2.field1());
		}

		it++;
		it2++;
	}
	
	REQUIRE(it2 == data2.end());
}

TEST_CASE("test_save_file")
{
	SaveFile file("test.dat");

	bool resultOpen = file.open(SaveFile::WRITE);
	REQUIRE(resultOpen);

	std::list<std::shared_ptr<SaveData>> testData = generateTestdata(50,50);

	bool writeSuccess = file.write(testData);
	REQUIRE(writeSuccess);

	bool closeSuccess = file.close();
	//REQUIRE(closeSuccess);

	SaveFile file2("test.dat");

	bool resultOpen2 = file2.open(SaveFile::READ);
	REQUIRE(resultOpen2);

	std::list<std::shared_ptr<SaveData>> testData2;
	bool readResult = file2.read(testData2);
	REQUIRE(readResult);

	compareTestData(testData, testData2);
}
