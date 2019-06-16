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

TEST_CASE("test_save_manager_save_load")
{
	auto data = generateTestdata(3, 5);

	const std::string file1 = "file1.dat", file2 = "file2.dat", file3 = "file3.dat";
	std::list<std::string> files{ {file1, file2, file3} };

	auto manager = ghost::SaveManager::create(".");

	auto it = data.begin();
	manager->addData(*it, file1);
	++it;
	manager->addData(*it, file2);
	++it;
	manager->addData(*it, file3);

	// save
	bool saveSuccess = manager->save(true);
	REQUIRE(saveSuccess);

	// load the save
	bool loadSuccess = manager->load(files);
	REQUIRE(loadSuccess);

	// check the data
	std::list<std::shared_ptr<ghost::SaveData>> resultData;

	it = data.begin();
	auto result1 = manager->getData((*it)->getName());
	REQUIRE(result1.size() == 1);
	REQUIRE(result1.count(file1) == 1);
	std::list<std::shared_ptr<ghost::SaveData>> tmp = result1.at(file1);
	resultData.insert(resultData.end(), tmp.begin(), tmp.end());

	++it;
	auto result2 = manager->getData((*it)->getName());
	REQUIRE(result2.size() == 1);
	REQUIRE(result2.count(file2) == 1);
	tmp = result2.at(file2);
	resultData.insert(resultData.end(), tmp.begin(), tmp.end());

	++it;
	auto result3 = manager->getData((*it)->getName());
	REQUIRE(result3.size() == 1);
	REQUIRE(result3.count(file3) == 1);
	tmp = result3.at(file3);
	resultData.insert(resultData.end(), tmp.begin(), tmp.end());

	compareTestData(data, resultData);
}

TEST_CASE("test_save_manager_save_not_overwrite")
{
	auto data = generateTestdata(3, 5);

	const std::string file1 = "file1.dat", file2 = "file2.dat", file3 = "file3.dat";
	std::list<std::string> files{ {file1, file2, file3} };

	auto manager = ghost::SaveManager::create(".");

	auto it = data.begin();
	manager->addData(*it, file1);
	++it;
	manager->addData(*it, file2);
	++it;
	manager->addData(*it, file3);

	// save
	bool saveSuccess = manager->save(true);
	REQUIRE(saveSuccess);

	bool saveSucces2 = manager->save(false);
	REQUIRE(!saveSucces2);
}

TEST_CASE("test_save_manager_files")
{
	auto data = generateTestdata(3, 5);

	const std::string file1 = "file1.dat", file2 = "file2.dat", file3 = "file3.dat";
	std::list<std::string> files{ {file1, file2, file3} };

	auto manager = ghost::SaveManager::create(".");

	auto it = data.begin();
	manager->addData(*it, file1);
	++it;
	manager->addData(*it, file2);
	++it;
	manager->addData(*it, file3);

	auto names = manager->getFileNames();
	REQUIRE(names.size() == 3);
	REQUIRE(names.front() == file1);
	REQUIRE(names.back() == file3);

	manager->removeData((*it)->getName(), "", true);

	auto names2 = manager->getFileNames();
	REQUIRE(names2.size() == 2);
	REQUIRE(names2.front() == file1);
	REQUIRE(names2.back() == file2);

	--it;
	manager->removeData((*it)->getName(), "", false);
	REQUIRE(names2.size() == 2);
	REQUIRE(names2.front() == file1);
	REQUIRE(names2.back() == file2);
}
