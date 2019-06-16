#include <gtest/gtest.h>
#include <iostream>

#include "../src/persistence/SaveFile.hpp"
#include "protobuf/tests.pb.h"

#include "PersistenceTestHelpers.hpp"

#include <ghost/persistence/SaveManager.hpp>

class SaveManagerTest : public testing::Test
{
protected:
	void SetUp() override
	{

	}

	void TearDown() override
	{

	}
};

TEST_F(SaveManagerTest, test_save_manager_single_data_add_get)
{
	auto manager = ghost::SaveManager::create("");

	auto data1 = std::make_shared<ghost::SaveData>("data1");

	// single data access
	manager->addData(data1, "file1");

	auto getResult = manager->getData(data1->getName());
	ASSERT_TRUE(getResult.size() == 1);
	ASSERT_TRUE(getResult.count("file1") == 1);

	const auto& list = getResult.at("file1");
	ASSERT_TRUE(list.size() == 1);
	ASSERT_TRUE(list.front()->getName() == data1->getName());
}

TEST_F(SaveManagerTest, test_save_manager_two_same_name)
{
	auto manager = ghost::SaveManager::create("");

	auto data1 = std::make_shared<ghost::SaveData>("data1");
	auto data2 = std::make_shared<ghost::SaveData>("data1");

	manager->addData(data1, "file1");
	manager->addData(data2, "file2");

	auto getResult = manager->getData(data1->getName());
	ASSERT_TRUE(getResult.size() == 2);
	ASSERT_TRUE(getResult.count("file1") == 1);
	ASSERT_TRUE(getResult.count("file2") == 1);

	const auto& list = getResult.at("file1");
	ASSERT_TRUE(list.size() == 1);
	ASSERT_TRUE(list.front()->getName() == data1->getName());

	const auto& list3 = getResult.at("file2");
	ASSERT_TRUE(list3.size() == 1);
	ASSERT_TRUE(list3.front()->getName() == data2->getName());

	// remove from one file
	bool removeSuccess = manager->removeData(data1->getName(), "file1");
	ASSERT_TRUE(removeSuccess);

	auto getResult2 = manager->getData(data1->getName());
	ASSERT_TRUE(getResult2.size() == 1);
	ASSERT_TRUE(getResult2.count("file1") == 0);
	ASSERT_TRUE(getResult2.count("file2") == 1);

	const auto& list2 = getResult2.at("file2");
	ASSERT_TRUE(list2.size() == 1);
	ASSERT_TRUE(list2.front()->getName() == data2->getName());

	manager->addData(data1, "file1");

	// remove everywhere!!!!!!!!!!!1
	bool removeSuccess2 = manager->removeData(data1->getName());
	ASSERT_TRUE(removeSuccess2);
	auto getResult3 = manager->getData(data1->getName());
	ASSERT_TRUE(getResult3.size() == 0);
}

TEST_F(SaveManagerTest, test_save_manager_save_load)
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
	ASSERT_TRUE(saveSuccess);

	// load the save
	bool loadSuccess = manager->load(files);
	ASSERT_TRUE(loadSuccess);

	// check the data
	std::list<std::shared_ptr<ghost::SaveData>> resultData;

	it = data.begin();
	auto result1 = manager->getData((*it)->getName());
	ASSERT_TRUE(result1.size() == 1);
	ASSERT_TRUE(result1.count(file1) == 1);
	std::list<std::shared_ptr<ghost::SaveData>> tmp = result1.at(file1);
	resultData.insert(resultData.end(), tmp.begin(), tmp.end());

	++it;
	auto result2 = manager->getData((*it)->getName());
	ASSERT_TRUE(result2.size() == 1);
	ASSERT_TRUE(result2.count(file2) == 1);
	tmp = result2.at(file2);
	resultData.insert(resultData.end(), tmp.begin(), tmp.end());

	++it;
	auto result3 = manager->getData((*it)->getName());
	ASSERT_TRUE(result3.size() == 1);
	ASSERT_TRUE(result3.count(file3) == 1);
	tmp = result3.at(file3);
	resultData.insert(resultData.end(), tmp.begin(), tmp.end());

	compareTestData(data, resultData);
}

TEST_F(SaveManagerTest, test_save_manager_save_not_overwrite)
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
	ASSERT_TRUE(saveSuccess);

	bool saveSucces2 = manager->save(false);
	ASSERT_TRUE(!saveSucces2);
}

TEST_F(SaveManagerTest, test_save_manager_files)
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
	ASSERT_TRUE(names.size() == 3);
	ASSERT_TRUE(names.front() == file1);
	ASSERT_TRUE(names.back() == file3);

	manager->removeData((*it)->getName(), "", true);

	auto names2 = manager->getFileNames();
	ASSERT_TRUE(names2.size() == 2);
	ASSERT_TRUE(names2.front() == file1);
	ASSERT_TRUE(names2.back() == file2);

	--it;
	manager->removeData((*it)->getName(), "", false);
	ASSERT_TRUE(names2.size() == 2);
	ASSERT_TRUE(names2.front() == file1);
	ASSERT_TRUE(names2.back() == file2);
}
