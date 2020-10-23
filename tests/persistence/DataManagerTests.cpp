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

#include <ghost/persistence/DataManager.hpp>
#include <ghost/persistence/DatabaseFile.hpp>
#include <iostream>

#include "../protobuf/tests.pb.h"
#include "PersistenceTestHelpers.hpp"

class DataManagerTest : public testing::Test
{
protected:
	void SetUp() override
	{
		_dataManager = ghost::DataManager::create();
		_dataManager->addDatabase(ghost::DatabaseFile::create(TEST_FILE_NAME), TEST_FILE_NAME);
	}

	void TearDown() override
	{
	}

	static const std::string TEST_DATA_NAME;
	static const std::string TEST_FILE_NAME;
	static const std::string TEST_FILE2_NAME;
	static const std::string TEST_FILE3_NAME;
	static const std::list<std::string> TEST_FILES_LIST;

	std::shared_ptr<ghost::DataManager> _dataManager;
};

const std::string DataManagerTest::TEST_DATA_NAME = "Test";
const std::string DataManagerTest::TEST_FILE_NAME = "file1.dat";
const std::string DataManagerTest::TEST_FILE2_NAME = "file2.dat";
const std::string DataManagerTest::TEST_FILE3_NAME = "file3.dat";
const std::list<std::string> DataManagerTest::TEST_FILES_LIST{{TEST_FILE_NAME, TEST_FILE2_NAME, TEST_FILE3_NAME}};

TEST_F(DataManagerTest, test_DataManager_addget_When_ok)
{
	auto data1 = _dataManager->addCollection(TEST_FILE_NAME, TEST_DATA_NAME);

	auto getResult = _dataManager->getCollections(data1->getName());
	ASSERT_TRUE(getResult.size() == 1);
	ASSERT_TRUE(getResult.count(TEST_FILE_NAME) == 1);

	const auto& list = getResult.at(TEST_FILE_NAME);
	ASSERT_TRUE(list.size() == 1);
	ASSERT_TRUE(list.front()->getName() == data1->getName());
}

TEST_F(DataManagerTest, test_DataManager_addgetWorks_When_twoSameNamesDifferentFiles)
{
	_dataManager->addDatabase(ghost::DatabaseFile::create(TEST_FILE2_NAME), TEST_FILE2_NAME);

	auto data1 = _dataManager->addCollection(TEST_FILE_NAME, TEST_DATA_NAME);
	auto data2 = _dataManager->addCollection(TEST_FILE2_NAME, TEST_DATA_NAME);

	auto getResult = _dataManager->getCollections(data1->getName());
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

TEST_F(DataManagerTest, test_DataManager_remove_When_ok)
{
	auto data1 = _dataManager->addCollection(TEST_FILE_NAME, TEST_DATA_NAME);

	bool removeSuccess = _dataManager->removeCollections(data1->getName());
	ASSERT_TRUE(removeSuccess);

	auto getResult2 = _dataManager->getCollections(data1->getName());
	ASSERT_TRUE(getResult2.count(TEST_FILE_NAME) == 0);
	ASSERT_TRUE(getResult2.size() == 0);
}

TEST_F(DataManagerTest, test_DataManager_remove_When_twoSameNamesDifferentFiles)
{
	_dataManager->addDatabase(ghost::DatabaseFile::create(TEST_FILE2_NAME), TEST_FILE2_NAME);

	auto data1 = _dataManager->addCollection(TEST_FILE_NAME, TEST_DATA_NAME);
	auto data2 = _dataManager->addCollection(TEST_FILE2_NAME, TEST_DATA_NAME);

	bool removeSuccess = _dataManager->removeCollections(data1->getName(), TEST_FILE_NAME);
	ASSERT_TRUE(removeSuccess);

	auto getResult2 = _dataManager->getCollections(data1->getName());
	ASSERT_TRUE(getResult2.count(TEST_FILE_NAME) == 0);
	ASSERT_TRUE(getResult2.size() == 1);

	const auto& list2 = getResult2.at(TEST_FILE2_NAME);
	ASSERT_TRUE(list2.size() == 1);
	ASSERT_TRUE(list2.front()->getName() == data2->getName());
}

TEST_F(DataManagerTest, test_DataManager_saveload_When_ok)
{
	_dataManager->addDatabase(ghost::DatabaseFile::create(TEST_FILE2_NAME), TEST_FILE2_NAME);
	_dataManager->addDatabase(ghost::DatabaseFile::create(TEST_FILE3_NAME), TEST_FILE3_NAME);

	auto data1 = generateTestdata(1, 5, _dataManager->getDatabase(TEST_FILE_NAME));
	auto data2 = generateTestdata(1, 5, _dataManager->getDatabase(TEST_FILE2_NAME));
	auto data3 = generateTestdata(1, 5, _dataManager->getDatabase(TEST_FILE3_NAME));

	// save
	for (auto name : _dataManager->getDatabaseNames())
	{
		auto file = std::dynamic_pointer_cast<ghost::DatabaseFile>(_dataManager->getDatabase(name));
		bool saveSuccess = file->save(true);
		ASSERT_TRUE(saveSuccess);
	}

	// load the save
	auto fileMap = ghost::DatabaseFile::load(".", TEST_FILES_LIST);
	ASSERT_EQ(fileMap.size(), 3);

	// check the data
	std::list<std::shared_ptr<ghost::internal::DataCollectionFile>> referenceData{data1.front(), data2.front(),
										      data3.front()};
	std::list<std::shared_ptr<ghost::internal::DataCollectionFile>> resultData;

	auto result = _dataManager->getCollections(data1.front()->getName());
	ASSERT_EQ(result.size(), 3);
	ASSERT_EQ(result.count(TEST_FILE_NAME), 1);
	ASSERT_EQ(result.count(TEST_FILE2_NAME), 1);
	ASSERT_EQ(result.count(TEST_FILE3_NAME), 1);
	std::list<std::shared_ptr<ghost::DataCollection>> tmp = result.at(TEST_FILE_NAME);
	// the save manager always returns instances of the internal version in this test, so it's okay to not check the
	// return value of the cast
	for (auto e : tmp) resultData.push_back(std::dynamic_pointer_cast<ghost::internal::DataCollectionFile>(e));

	tmp = result.at(TEST_FILE2_NAME);
	// the save manager always returns instances of the internal version in this test, so it's okay to not check the
	// return value of the cast
	for (auto e : tmp) resultData.push_back(std::dynamic_pointer_cast<ghost::internal::DataCollectionFile>(e));

	tmp = result.at(TEST_FILE3_NAME);
	// the save manager always returns instances of the internal version in this test, so it's okay to not check the
	// return value of the cast
	for (auto e : tmp) resultData.push_back(std::dynamic_pointer_cast<ghost::internal::DataCollectionFile>(e));

	compareTestData(referenceData, resultData);
}

TEST_F(DataManagerTest, test_DataManager_save_When_noOverwrite)
{
	_dataManager->addDatabase(ghost::DatabaseFile::create(TEST_FILE2_NAME), TEST_FILE2_NAME);
	_dataManager->addDatabase(ghost::DatabaseFile::create(TEST_FILE3_NAME), TEST_FILE3_NAME);

	auto data1 = generateTestdata(1, 5, _dataManager->getDatabase(TEST_FILE_NAME));
	auto data2 = generateTestdata(1, 5, _dataManager->getDatabase(TEST_FILE2_NAME));
	auto data3 = generateTestdata(1, 5, _dataManager->getDatabase(TEST_FILE3_NAME));

	// save
	for (auto name : _dataManager->getDatabaseNames())
	{
		auto file = std::dynamic_pointer_cast<ghost::DatabaseFile>(_dataManager->getDatabase(name));
		bool saveSuccess = file->save(true);
		ASSERT_TRUE(saveSuccess);

		bool saveSuccess2 = file->save(false);
		ASSERT_FALSE(saveSuccess2);
	}
}

TEST_F(DataManagerTest, test_DataManager_getFileNames_When_ok)
{
	_dataManager->addDatabase(ghost::DatabaseFile::create(TEST_FILE2_NAME), TEST_FILE2_NAME);
	_dataManager->addDatabase(ghost::DatabaseFile::create(TEST_FILE3_NAME), TEST_FILE3_NAME);

	auto data1 = generateTestdata(1, 5, _dataManager->getDatabase(TEST_FILE_NAME));
	auto data2 = generateTestdata(1, 5, _dataManager->getDatabase(TEST_FILE2_NAME));
	auto data3 = generateTestdata(1, 5, _dataManager->getDatabase(TEST_FILE3_NAME));

	auto names = _dataManager->getDatabaseNames();
	ASSERT_TRUE(names.size() == 3);
	ASSERT_TRUE(names.front() == TEST_FILE_NAME);
	ASSERT_TRUE(names.back() == TEST_FILE3_NAME);
}
