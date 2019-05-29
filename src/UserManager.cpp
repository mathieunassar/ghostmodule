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

#include "UserManager.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::UserManager> ghost::UserManager::create()
{
	return std::shared_ptr<ghost::UserManager>(new ghost::internal::UserManager());
}

UserManager::UserManager()
{
	(void)createUser("superuser", "superuser");
}

void UserManager::addUserToGroup(const ghost::User& user, ghost::UserGroup& group)
{
	for (auto u : _users)
	{
		if (u.get() == &user)
		{
			for (auto g : _groups)
			{
				if (g.get() == &group)
					g->addUser(u);
			}
		}
	}
}

std::shared_ptr<ghost::User> UserManager::createUser(const std::string& name, const std::string& password)
{
	for (auto& user : _users)
	{
		if (user->getName() == name)
			return nullptr;
	}

	auto newUser = std::make_shared<ghost::internal::User>(name, password);
	_users.emplace_back(newUser);
	return newUser;
}

std::shared_ptr<ghost::UserGroup> UserManager::createUserGroup(const std::string& name)
{
	for (auto& group : _groups)
	{
		if (group->getName() == name)
			return nullptr;
	}

	auto newGroup = std::make_shared<ghost::internal::UserGroup>(name);
	_groups.emplace_back(newGroup);
	return newGroup;
}

bool UserManager::connect(const std::string& username, const std::string& password)
{
	for (auto& user : _users)
	{
		if (user->getName() == username && user->isPasswordCorrect(password))
		{
			_connectedUser = user;

			if (_connectedUserCallback)
				_connectedUserCallback(user);

			return true;
		}
	}
	return false;
}

void UserManager::disconnect()
{
	_connectedUser = nullptr;
}

bool UserManager::isUserConnected() const
{
	return _connectedUser.operator bool();
}

std::shared_ptr<ghost::User> UserManager::getConnectedUser() const
{
	return _connectedUser;
}

void UserManager::setConnectedUserCallback(std::function<void(std::shared_ptr<ghost::User>)> callback)
{
	_connectedUserCallback = callback;
}

std::vector<std::shared_ptr<ghost::UserGroup>> UserManager::getUserGroups() const
{
	return std::vector<std::shared_ptr<ghost::UserGroup>>(_groups.begin(), _groups.end());
}
