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

#include "../src/module/UserManager.hpp"

class UserManagerTest : public testing::Test
{
protected:
	void SetUp() override
	{
		_callbackWasCalled = false;
		_manager = std::make_shared<ghost::internal::UserManager>();
	}

	void TearDown() override
	{
	}

	bool _callbackWasCalled;
	std::shared_ptr<ghost::User> _callbackUser;
	std::shared_ptr<ghost::UserManager> _manager;

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
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	ASSERT_TRUE(newUser->getName() == TEST_USERNAME);
}

TEST_F(UserManagerTest, Test_UserManager_createUser_When_userAlreadyExists)
{
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	std::shared_ptr<ghost::User> newUser2 = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_FALSE(newUser2);
}

TEST_F(UserManagerTest, Test_UserManager_createGroup_When_ok)
{
	auto newGroup = _manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);
	ASSERT_TRUE(newGroup->getName() == TEST_GROUPNAME);
}

TEST_F(UserManagerTest, Test_UserManager_createGroup_When_groupAlreadyExists)
{
	auto newGroup = _manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);

	auto newGroup2 = _manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_FALSE(newGroup2);
}

TEST_F(UserManagerTest, Test_UserManager_addUserToGroup_When_ok)
{
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	auto newGroup = _manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);

	ASSERT_FALSE(newGroup->containsUser(*newUser));
	_manager->addUserToGroup(*newUser, *newGroup);
	ASSERT_TRUE(newGroup->containsUser(*newUser));
	ASSERT_TRUE(newGroup->contains(*newUser));
}

TEST_F(UserManagerTest, Test_UserManager_addUserToGroup_When_groupNotFound)
{
	auto manager2 = ghost::UserManager::create();

	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	auto newGroup = manager2->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);

	ASSERT_FALSE(newGroup->containsUser(*newUser));
	_manager->addUserToGroup(*newUser, *newGroup);
	ASSERT_FALSE(newGroup->containsUser(*newUser));
	ASSERT_FALSE(newGroup->contains(*newUser));
}

TEST_F(UserManagerTest, Test_UserManager_connect_When_ok)
{
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	bool connectSuccess = _manager->connect(TEST_USERNAME, TEST_PASSWORD, ghost::Session::createLocal());
	ASSERT_TRUE(connectSuccess);
	ASSERT_TRUE(_manager->getConnectedUser(ghost::Session::createLocal()));
	ASSERT_TRUE(_manager->getConnectedUser(ghost::Session::createLocal())->getName() == TEST_USERNAME);
	ASSERT_TRUE(_manager->getConnectedUser(ghost::Session::createLocal()).get() == newUser.get());
	ASSERT_TRUE(_manager->isUserConnected(ghost::Session::createLocal()));
}

TEST_F(UserManagerTest, Test_UserManager_connect_When_userNotFound)
{
	bool connectSuccess = _manager->connect(TEST_USERNAME, TEST_PASSWORD, ghost::Session::createLocal());
	ASSERT_FALSE(connectSuccess);
	ASSERT_FALSE(_manager->getConnectedUser(ghost::Session::createLocal()));
	ASSERT_FALSE(_manager->isUserConnected(ghost::Session::createLocal()));
}

TEST_F(UserManagerTest, Test_UserManager_connect_When_wrongPassword)
{
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	bool connectSuccess = _manager->connect(TEST_USERNAME, TEST_PASSWORD2, ghost::Session::createLocal());
	ASSERT_FALSE(connectSuccess);
	ASSERT_FALSE(_manager->getConnectedUser(ghost::Session::createLocal()));
	ASSERT_FALSE(_manager->isUserConnected(ghost::Session::createLocal()));
}

TEST_F(UserManagerTest, Test_UserManager_isConnectedInSession_When_sessionIsProvided)
{
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	auto session = ghost::Session::create();
	_manager->connect(TEST_USERNAME, TEST_PASSWORD, session);

	ASSERT_TRUE(_manager->isUserConnected(session));
	ASSERT_FALSE(_manager->isUserConnected(ghost::Session::createLocal()));
	ASSERT_FALSE(_manager->isUserConnected(ghost::Session::create()));
}

TEST_F(UserManagerTest, Test_UserManager_connect_When_callbackIsSet)
{
	_manager->setConnectedUserCallback(
	    std::bind(&UserManagerTest::userConnectedCallback, this, std::placeholders::_1),
	    ghost::Session::createLocal());
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	ASSERT_FALSE(_callbackWasCalled);
	bool connectSuccess = _manager->connect(TEST_USERNAME, TEST_PASSWORD, ghost::Session::createLocal());
	ASSERT_TRUE(connectSuccess);
	ASSERT_TRUE(_callbackWasCalled);
	ASSERT_TRUE(_callbackUser.get() == newUser.get());
}

TEST_F(UserManagerTest, Test_UserManager_connectionFails_When_userAlreadyConnected)
{
	_manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	auto session = ghost::Session::create();
	auto session2 = ghost::Session::create();
	_manager->connect(TEST_USERNAME, TEST_PASSWORD, session);
	bool secondConnectionResult = _manager->connect(TEST_USERNAME, TEST_PASSWORD, session2);

	ASSERT_FALSE(secondConnectionResult);
	ASSERT_TRUE(_manager->isUserConnected(session));
}

TEST_F(UserManagerTest, Test_UserManager_twoUsersAreConnected)
{
	_manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	_manager->createUser(TEST_USERNAME2, TEST_PASSWORD2);
	auto session = ghost::Session::create();
	auto session2 = ghost::Session::create();
	_manager->connect(TEST_USERNAME, TEST_PASSWORD, session);
	_manager->connect(TEST_USERNAME2, TEST_PASSWORD2, session2);

	ASSERT_TRUE(_manager->isUserConnected(session));
	ASSERT_FALSE(_manager->isUserConnected(ghost::Session::createLocal()));
	ASSERT_TRUE(_manager->isUserConnected(session2));
}

TEST_F(UserManagerTest, Test_UserManager_sessionCallbackIsCalled_When_userConnects)
{
	_manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	auto session = ghost::Session::create();

	_manager->setConnectedUserCallback(
	    std::bind(&UserManagerTest::userConnectedCallback, this, std::placeholders::_1), session);

	_manager->connect(TEST_USERNAME, TEST_PASSWORD, session);

	ASSERT_TRUE(_callbackWasCalled);
}

TEST_F(UserManagerTest, Test_UserManager_sessionCallbackIsNotCalled_When_userConnectsWithOtherSession)
{
	_manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	auto session = ghost::Session::create();

	_manager->setConnectedUserCallback(
	    std::bind(&UserManagerTest::userConnectedCallback, this, std::placeholders::_1), session);

	_manager->connect(TEST_USERNAME, TEST_PASSWORD, ghost::Session::createLocal());

	ASSERT_FALSE(_callbackWasCalled);
}

TEST_F(UserManagerTest, Test_UserManager_disconnect_When_ok)
{
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	bool connectSuccess = _manager->connect(TEST_USERNAME, TEST_PASSWORD, ghost::Session::createLocal());
	ASSERT_TRUE(connectSuccess);

	_manager->disconnect(ghost::Session::createLocal());
	ASSERT_FALSE(_manager->getConnectedUser(ghost::Session::createLocal()));
	ASSERT_FALSE(_manager->isUserConnected(ghost::Session::createLocal()));
}

TEST_F(UserManagerTest, Test_UserManager_disconnect_When_callbackIsSet)
{
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);

	bool connectSuccess = _manager->connect(TEST_USERNAME, TEST_PASSWORD, ghost::Session::createLocal());
	ASSERT_TRUE(connectSuccess);

	_manager->setConnectedUserCallback(
	    std::bind(&UserManagerTest::userConnectedCallback, this, std::placeholders::_1),
	    ghost::Session::createLocal());

	_manager->disconnect(ghost::Session::createLocal());
	ASSERT_FALSE(_manager->getConnectedUser(ghost::Session::createLocal()));
	ASSERT_FALSE(_manager->isUserConnected(ghost::Session::createLocal()));

	ASSERT_TRUE(_callbackWasCalled);
	ASSERT_FALSE(_callbackUser);
}

TEST_F(UserManagerTest, Test_UserManager_sessionCallbackIsCalled_When_userDisconnects)
{
	_manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	auto session = ghost::Session::create();
	_manager->connect(TEST_USERNAME, TEST_PASSWORD, session);

	_manager->setConnectedUserCallback(
	    std::bind(&UserManagerTest::userConnectedCallback, this, std::placeholders::_1), session);
	_manager->disconnect(session);

	ASSERT_TRUE(_callbackWasCalled);
}

TEST_F(UserManagerTest, Test_UserManager_sessionCallbackIsNotCalled_When_userDisonnectsWithOtherSession)
{
	_manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	auto session = ghost::Session::create();
	_manager->connect(TEST_USERNAME, TEST_PASSWORD, ghost::Session::createLocal());

	_manager->setConnectedUserCallback(
	    std::bind(&UserManagerTest::userConnectedCallback, this, std::placeholders::_1), session);
	_manager->disconnect(ghost::Session::createLocal());

	ASSERT_FALSE(_callbackWasCalled);
}

TEST_F(UserManagerTest, Test_UserManager_getGroups_When_ok)
{
	std::shared_ptr<ghost::User> newUser = _manager->createUser(TEST_USERNAME, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	std::shared_ptr<ghost::User> newUser2 = _manager->createUser(TEST_USERNAME2, TEST_PASSWORD);
	ASSERT_TRUE(newUser);
	auto newGroup = _manager->createUserGroup(TEST_GROUPNAME);
	ASSERT_TRUE(newGroup);

	_manager->addUserToGroup(*newUser, *newGroup);

	auto groups = _manager->getUserGroups();
	ASSERT_TRUE(groups.size() == 1);
	ASSERT_TRUE(groups.front()->getUsers().size() == 1);
	ASSERT_TRUE(groups.front()->getUsers().front()->getName() == TEST_USERNAME);

	_manager->addUserToGroup(*newUser2, *newGroup);

	ASSERT_TRUE(groups.front()->getUsers().size() == 2);
	ASSERT_TRUE(groups.front()->getUsers().back()->getName() == TEST_USERNAME2);
}
