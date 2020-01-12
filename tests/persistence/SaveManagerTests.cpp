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

#include <ghost/persistence/SaveManager.hpp>
#include <iostream>

#include "../../src/persistence/SaveFile.hpp"
#include "../protobuf/tests.pb.h"
#include "PersistenceTestHelpers.hpp"

class SaveManagerTest : public testing::Test
{
protected:
	void SetUp() override
	{
	}

	void TearDown() override
	{
	}

	static const std::string TEST_DATA_NAME;
	static const std::string TEST_FILE_NAME;
	static const std::string TEST_FILE2_NAME;
	static const std::string TEST_FILE3_NAME;
	static const std::list<std::string> TEST_FILES_LIST;
};

const std::string SaveManagerTest::TEST_DATA_NAME = "Test";
const std::string SaveManagerTest::TEST_FILE_NAME = "file1.dat";
const std::string SaveManagerTest::TEST_FILE2_NAME = "file2.dat";
const std::string SaveManagerTest::TEST_FILE3_NAME = "file3.dat";
const std::list<std::string> SaveManagerTest::TEST_FILES_LIST{{TEST_FILE_NAME, TEST_FILE2_NAME, TEST_FILE3_NAME}};

TEST_F(SaveManagerTest, test_SaveManager_addget_When_ok)
{
	auto manager = ghost::SaveManager::create("");

	auto data1 = ghost::SaveData::create(TEST_DATA_NAME);

	// single data access
	manager->addData(data1, TEST_FILE_NAME);

	auto getResult = manager->getData(data1->getName());
	ASSERT_TRUE(getResult.size() == 1);
	ASSERT_TRUE(getResult.count(TEST_FILE_NAME) == 1);

	const auto& list = getResult.at(TEST_FILE_NAME);
	ASSERT_TRUE(list.size() == 1);
	ASSERT_TRUE(list.front()->getName() == data1->getName());
}

TEST_F(SaveManagerTest, test_SaveManager_addget_When_twoSameNamesDifferentFiles)
{
	auto manager = ghost::SaveManager::create("");

	auto data1 = ghost::SaveData::create(TEST_DATA_NAME);
	auto data2 = ghost::SaveData::create(TEST_DATA_NAME);

	manager->addData(data1, TEST_FILE_NAME);
	manager->addData(data2, TEST_FILE2_NAME);

	auto getResult = manager->getData(data1->getName());
	ASSERT_TRUE(getResult.size() == 2);
	ASSERT_TRUE(getResult.count(TEST_FILE_NAME) == 1);
	ASSERT_TRUE(getResult.count(TEST_FILE2_NAME) == 1);

	const auto& list = getResult.at(TEST_FILE_NAME);
	ASSERT_TRUE(list.size() == 1);
	ASSERT_TRUE(list.front()->getName() == data1->getName());

	const auto& list3 = getResult.at(TEST_FILE2_NAME);
	ASSERT_TRUE(list3.size() == 1);
	ASSERT_TRUE(list3.front()->getName() == data2->getName());
}

TEST_F(SaveManagerTest, test_SaveManager_remove_When_ok)
{
	auto manager = ghost::SaveManager::create("");

	auto data1 = ghost::SaveData::create(TEST_DATA_NAME);
	manager->addData(data1, TEST_FILE_NAME);

	bool removeSuccess = manager->removeData(data1->getName());
	ASSERT_TRUE(removeSuccess);

	auto getResult2 = manager->getData(data1->getName());
	ASSERT_TRUE(getResult2.count(TEST_FILE_NAME) == 0);
	ASSERT_TRUE(getResult2.size() == 0);
}

TEST_F(SaveManagerTest, test_SaveManager_remove_When_twoSameNamesDifferentFiles)
{
	auto manager = ghost::SaveManager::create("");

	auto data1 = ghost::SaveData::create(TEST_DATA_NAME);
	auto data2 = ghost::SaveData::create(TEST_DATA_NAME);

	manager->addData(data1, TEST_FILE_NAME);
	manager->addData(data2, TEST_FILE2_NAME);

	bool removeSuccess = manager->removeData(data1->getName(), TEST_FILE_NAME);
	ASSERT_TRUE(removeSuccess);

	auto getResult2 = manager->getData(data1->getName());
	ASSERT_TRUE(getResult2.count(TEST_FILE_NAME) == 0);
	ASSERT_TRUE(getResult2.size() == 1);

	const auto& list2 = getResult2.at(TEST_FILE2_NAME);
	ASSERT_TRUE(list2.size() == 1);
	ASSERT_TRUE(list2.front()->getName() == data2->getName());
}

TEST_F(SaveManagerTest, test_SaveManager_saveload_When_ok)
{
	auto data = generateTestdata(3, 5);

	auto manager = ghost::SaveManager::create(".");

	auto it = data.begin();
	manager->addData(*it, TEST_FILE_NAME);
	++it;
	manager->addData(*it, TEST_FILE2_NAME);
	++it;
	manager->addData(*it, TEST_FILE3_NAME);

	// save
	bool saveSuccess = manager->save(true);
	ASSERT_TRUE(saveSuccess);

	// load the save
	bool loadSuccess = manager->load(TEST_FILES_LIST);
	ASSERT_TRUE(loadSuccess);

	// check the data
	std::list<std::shared_ptr<ghost::internal::SaveData>> resultData;

	it = data.begin();
	auto result1 = manager->getData((*it)->getName());
	ASSERT_TRUE(result1.size() == 1);
	ASSERT_TRUE(result1.count(TEST_FILE_NAME) == 1);
	std::list<std::shared_ptr<ghost::SaveData>> tmp = result1.at(TEST_FILE_NAME);
	// the save manager always returns instances of the internal version in this test, so it's okay to not check the
	// return value of the cast
	for (auto e : tmp) resultData.push_back(std::dynamic_pointer_cast<ghost::internal::SaveData>(e));

	++it;
	auto result2 = manager->getData((*it)->getName());
	ASSERT_TRUE(result2.size() == 1);
	ASSERT_TRUE(result2.count(TEST_FILE2_NAME) == 1);
	tmp = result2.at(TEST_FILE2_NAME);
	// the save manager always returns instances of the internal version in this test, so it's okay to not check the
	// return value of the cast
	for (auto e : tmp) resultData.push_back(std::dynamic_pointer_cast<ghost::internal::SaveData>(e));

	++it;
	auto result3 = manager->getData((*it)->getName());
	ASSERT_TRUE(result3.size() == 1);
	ASSERT_TRUE(result3.count(TEST_FILE3_NAME) == 1);
	tmp = result3.at(TEST_FILE3_NAME);
	// the save manager always returns instances of the internal version in this test, so it's okay to not check the
	// return value of the cast
	for (auto e : tmp) resultData.push_back(std::dynamic_pointer_cast<ghost::internal::SaveData>(e));

	compareTestData(data, resultData);
}

TEST_F(SaveManagerTest, test_SaveManager_save_When_noOverwrite)
{
	auto data = generateTestdata(3, 5);

	auto manager = ghost::SaveManager::create(".");

	auto it = data.begin();
	manager->addData(*it, TEST_FILE_NAME);
	++it;
	manager->addData(*it, TEST_FILE2_NAME);
	++it;
	manager->addData(*it, TEST_FILE3_NAME);

	// save
	bool saveSuccess = manager->save(true);
	ASSERT_TRUE(saveSuccess);

	bool saveSucces2 = manager->save(false);
	ASSERT_TRUE(!saveSucces2);
}

TEST_F(SaveManagerTest, test_SaveManager_getFileNames_When_ok)
{
	auto data = generateTestdata(3, 5);

	auto manager = ghost::SaveManager::create(".");

	auto it = data.begin();
	manager->addData(*it, TEST_FILE_NAME);
	++it;
	manager->addData(*it, TEST_FILE2_NAME);
	++it;
	manager->addData(*it, TEST_FILE3_NAME);

	auto names = manager->getFileNames();
	ASSERT_TRUE(names.size() == 3);
	ASSERT_TRUE(names.front() == TEST_FILE_NAME);
	ASSERT_TRUE(names.back() == TEST_FILE3_NAME);

	manager->removeData((*it)->getName(), "", true); // remove TEST_FILE3_NAME

	auto names2 = manager->getFileNames();
	ASSERT_TRUE(names2.size() == 2);
	ASSERT_TRUE(names2.front() == TEST_FILE_NAME);
	ASSERT_TRUE(names2.back() == TEST_FILE2_NAME);
}

TEST_F(SaveManagerTest, test_SaveManager_getFileNames_When_updateAfterResultIsGotten)
{
	auto data = generateTestdata(1, 5);

	auto manager = ghost::SaveManager::create(".");

	auto fileNames = manager->getFileNames();
	ASSERT_TRUE(fileNames.size() == 0);

	manager->addData(data.front(), TEST_FILE_NAME);
	ASSERT_TRUE(fileNames.size() == 0); // should not change
}