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

#include "../src/module/UserManager.hpp"

class UserManagerTest : public testing::Test
{
protected:
	void SetUp() override
	{
		_callbackWasCalled = false;
	}

	void TearDown() override
	{
	}

	bool _callbackWasCalled;
	std::shared_ptr<ghost::User> _callbackUser;

	static const std::string TEST_USERNAME;
	static const std::string TEST_USERNAME2;
	static const std::string TEST_PASSWORD;
	static const std::string TEST_PASSWORD2;
	static const std::string TEST_GROUPNAME;

public:
	void userConnectedCallback(std::shared_ptr<ghost::User> newUser)
	{
		_callbackWasCalled = true;
		_callbackUser = newUser;
	}
};

const std::string UserManagerTest::TEST_USERNAME = "username";
const std::string UserManagerTest::TEST_USERNAME2 = "username2";
const std::string UserManagerTest::TEST_PASSWORD = "password";
const std::string UserManagerTest::TEST_PASSWORD2 = "password2";
const std::string UserManagerTest::TEST_GROUPNAME = "group";

TEST_F(UserManagerTest, Test_UserManager_createUser_When_ok)
{
	auto manager = ghost::UserManager::create();
	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	ASSERT_TRUE(newUser->getName() == TEST_USERNAME);
}

TEST_F(UserManagerTest, Test_UserManager_createUser_When_userAlreadyExists)
{
	auto manager = ghost::UserManager::create();
	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	std::shared_ptr<ghost::User> newUser2 = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_FALSE(newUser2);
}

TEST_F(UserManagerTest, Test_UserManager_createGroup_When_ok)
{
	auto manager = ghost::UserManager::create();
	auto newGroup = manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);
	ASSERT_TRUE(newGroup->getName() == TEST_GROUPNAME);
}

TEST_F(UserManagerTest, Test_UserManager_createGroup_When_groupAlreadyExists)
{
	auto manager = ghost::UserManager::create();
	auto newGroup = manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);

	auto newGroup2 = manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_FALSE(newGroup2);
}

TEST_F(UserManagerTest, Test_UserManager_addUserToGroup_When_ok)
{
	auto manager = ghost::UserManager::create();

	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	auto newGroup = manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);

	ASSERT_FALSE(newGroup->containsUser(*newUser));
	manager->addUserToGroup(*newUser, *newGroup);
	ASSERT_TRUE(newGroup->containsUser(*newUser));
	ASSERT_TRUE(newGroup->contains(*newUser));
}

TEST_F(UserManagerTest, Test_UserManager_addUserToGroup_When_groupNotFound)
{
	auto manager = ghost::UserManager::create();
	auto manager2 = ghost::UserManager::create();

	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	auto newGroup = manager2->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);

	ASSERT_FALSE(newGroup->containsUser(*newUser));
	manager->addUserToGroup(*newUser, *newGroup);
	ASSERT_FALSE(newGroup->containsUser(*newUser));
	ASSERT_FALSE(newGroup->contains(*newUser));
}

TEST_F(UserManagerTest, Test_UserManager_connect_When_ok)
{
	auto manager = ghost::UserManager::create();
	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	bool connectSuccess = manager->connect(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(connectSuccess);
	ASSERT_TRUE(manager->getConnectedUser());
	ASSERT_TRUE(manager->getConnectedUser()->getName() == TEST_USERNAME);
	ASSERT_TRUE(manager->getConnectedUser().get() == newUser.get());
	ASSERT_TRUE(manager->isUserConnected());
}

TEST_F(UserManagerTest, Test_UserManager_connect_When_userNotFound)
{
	auto manager = ghost::UserManager::create();

	bool connectSuccess = manager->connect(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_FALSE(connectSuccess);
	ASSERT_FALSE(manager->getConnectedUser());
	ASSERT_FALSE(manager->isUserConnected());
}

TEST_F(UserManagerTest, Test_UserManager_connect_When_wrongPassword)
{
	auto manager = ghost::UserManager::create();
	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	bool connectSuccess = manager->connect(TEST_USERNAME, TEST_PASSWORD2);
	ASSERT_FALSE(connectSuccess);
	ASSERT_FALSE(manager->getConnectedUser());
	ASSERT_FALSE(manager->isUserConnected());
}

TEST_F(UserManagerTest, Test_UserManager_connect_When_callbackIsSet)
{
	auto manager = ghost::UserManager::create();
	manager->setConnectedUserCallback(
	    std::bind(&UserManagerTest::userConnectedCallback, this, std::placeholders::_1));
	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	ASSERT_FALSE(_callbackWasCalled);
	bool connectSuccess = manager->connect(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(connectSuccess);
	ASSERT_TRUE(_callbackWasCalled);
	ASSERT_TRUE(_callbackUser.get() == newUser.get());
}

TEST_F(UserManagerTest, Test_UserManager_disconnect_When_ok)
{
	auto manager = ghost::UserManager::create();
	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	bool connectSuccess = manager->connect(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(connectSuccess);

	manager->disconnect();
	ASSERT_FALSE(manager->getConnectedUser());
	ASSERT_FALSE(manager->isUserConnected());
}

TEST_F(UserManagerTest, Test_UserManager_disconnect_When_callbackIsSet)
{
	auto manager = ghost::UserManager::create();
	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	bool connectSuccess = manager->connect(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(connectSuccess);

	manager->setConnectedUserCallback(
	    std::bind(&UserManagerTest::userConnectedCallback, this, std::placeholders::_1));

	manager->disconnect();
	ASSERT_FALSE(manager->getConnectedUser());
	ASSERT_FALSE(manager->isUserConnected());

	ASSERT_TRUE(_callbackWasCalled);
	ASSERT_FALSE(_callbackUser);
}

TEST_F(UserManagerTest, Test_UserManager_getGroups_When_ok)
{
	auto manager = ghost::UserManager::create();

	std::shared_ptr<ghost::User> newUser = manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	std::shared_ptr<ghost::User> newUser2 = manager->createUser(TEST_USERNAME2, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	auto newGroup = manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);

	manager->addUserToGroup(*newUser, *newGroup);

	auto groups = manager->getUserGroups();
	ASSERT_TRUE(groups.size() == 1);
	ASSERT_TRUE(groups.front()->getUsers().size() == 1);
	ASSERT_TRUE(groups.front()->getUsers().front()->getName() == TEST_USERNAME);

	manager->addUserToGroup(*newUser2, *newGroup);

	ASSERT_TRUE(groups.front()->getUsers().size() == 2);
	ASSERT_TRUE(groups.front()->getUsers().back()->getName() == TEST_USERNAME2);
}
