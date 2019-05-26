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
#include <stdexcept>

using namespace ghost::internal;

std::shared_ptr<ghost::UserManager> ghost::UserManager::create()
{
	return std::shared_ptr<ghost::UserManager>(new ghost::internal::UserManager());
}

UserManager::UserManager()
{
	createUser("superuser", "superuser");
}

void UserManager::addUserToGroup(const ghost::User& user, ghost::UserGroup& group)
{

}

bool UserManager::createUser(const std::string& name, const std::string& password)
{
	for (auto& user : _users)
	{
		if (user->getName() == name)
			return false;
	}

	_users.emplace_back(new User(name, password));
	return true;
}

bool UserManager::createUserGroup(const std::string& name)
{
	for (auto& group : _groups)
	{
		if (group->getName() == name)
			return false;
	}

	_groups.emplace_back(new UserGroup(name));
	return true;
}

bool UserManager::connect(const std::string& username, const std::string& password)
{
	for (auto& user : _users)
	{
		if (user->getName() == username && user->isPasswordCorrect(password))
		{
			_connectedUser = user;

			if (_connectedUserCallback)
				_connectedUserCallback(*user);

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

User& UserManager::getConnectedUser() const
{
	if (!_connectedUser)
		throw std::logic_error("No connected user.");

	return *_connectedUser;
}

void UserManager::setConnectedUserCallback(std::function<void(const ghost::User&)> callback)
{
	_connectedUserCallback = callback;
}

std::vector<std::shared_ptr<ghost::UserGroup>> UserManager::getUserGroups() const
{
	return std::vector<std::shared_ptr<ghost::UserGroup>>(_groups.begin(), _groups.end());
}
