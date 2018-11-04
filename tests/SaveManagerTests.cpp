#include <catch.hpp>
#include <iostream>

#include <ghost/persistence/internal/SaveFile.hpp>
#include "protobuf/tests.pb.h"

#include "PersistenceTestHelpers.hpp"

#include <ghost/persistence/SaveManager.hpp>

TEST_CASE("test_save_manager_single_data_add_get")
{
	auto manager = ghost::SaveManager::create("");

	auto data1 = std::make_shared<ghost::SaveData>("data1");

	// single data access
	manager->addData(data1, "file1");

	auto getResult = manager->getData(data1->getName());
	REQUIRE(getResult.size() == 1);
	REQUIRE(getResult.count("file1") == 1);

	const auto& list = getResult.at("file1");
	REQUIRE(list.size() == 1);
	REQUIRE(list.front()->getName() == data1->getName());
}

TEST_CASE("test_save_manager_two_same_name")
{
	auto manager = ghost::SaveManager::create("");

	auto data1 = std::make_shared<ghost::SaveData>("data1");
	auto data2 = std::make_shared<ghost::SaveData>("data1");

	manager->addData(data1, "file1");
	manager->addData(data2, "file2");

	auto getResult = manager->getData(data1->getName());
	REQUIRE(getResult.size() == 2);
	REQUIRE(getResult.count("file1") == 1);
	REQUIRE(getResult.count("file2") == 1);

	const auto& list = getResult.at("file1");
	REQUIRE(list.size() == 1);
	REQUIRE(list.front()->getName() == data1->getName());

	const auto& list3 = getResult.at("file2");
	REQUIRE(list3.size() == 1);
	REQUIRE(list3.front()->getName() == data2->getName());

	// remove from one file
	bool removeSuccess = manager->removeData(data1->getName(), "file1");
	REQUIRE(removeSuccess);

	auto getResult2 = manager->getData(data1->getName());
	REQUIRE(getResult2.size() == 1);
	REQUIRE(getResult2.count("file1") == 0);
	REQUIRE(getResult2.count("file2") == 1);

	const auto& list2 = getResult2.at("file2");
	REQUIRE(list2.size() == 1);
	REQUIRE(list2.front()->getName() == data2->getName());

	manager->addData(data1, "file1");

	// remove everywhere!!!!!!!!!!!1
	bool removeSuccess2 = manager->removeData(data1->getName());
	REQUIRE(removeSuccess2);
	auto getResult3 = manager->getData(data1->getName());
	REQUIRE(getResult3.size() == 0);
}
