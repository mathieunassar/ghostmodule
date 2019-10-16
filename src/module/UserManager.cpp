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
				if (g.get() == &group) g->addUser(u);
			}
		}
	}
}

std::shared_ptr<ghost::User> UserManager::createUser(const std::string& name, const std::string& password)
{
	for (auto& user : _users)
	{
		if (user->getName() == name) return nullptr;
	}

	auto newUser = std::make_shared<ghost::internal::User>(name, password);
	_users.emplace_back(newUser);
	return newUser;
}

std::shared_ptr<ghost::UserGroup> UserManager::createUserGroup(const std::string& name)
{
	for (auto& group : _groups)
	{
		if (group->getName() == name) return nullptr;
	}

	auto newGroup = std::make_shared<ghost::internal::UserGroup>(name);
	_groups.emplace_back(newGroup);
	return newGroup;
}

bool UserManager::connect(const std::string& username, const std::string& password,
			  const std::shared_ptr<ghost::Session>& session)
{
	for (auto& user : _users)
	{
		if (user->getName() == username && user->isPasswordCorrect(password))
		{
			// The connection fails if the user is already connected in any session
			if (isUserAlreadyConnected(user)) return false;

			_connectedUsers[session->getUUID()] = user;

			if (_connectedUserCallbacks.find(session->getUUID()) != _connectedUserCallbacks.end())
				_connectedUserCallbacks[session->getUUID()](user);

			return true;
		}
	}
	return false;
}

void UserManager::disconnect(const std::shared_ptr<ghost::Session>& session)
{
	_connectedUsers.erase(session->getUUID());

	// Report to the callback that the user disconnected
	if (_connectedUserCallbacks.find(session->getUUID()) != _connectedUserCallbacks.end())
		_connectedUserCallbacks[session->getUUID()](nullptr);
}

bool UserManager::isUserConnected(const std::shared_ptr<ghost::Session>& session) const
{
	return _connectedUsers.find(session->getUUID()) != _connectedUsers.end();
}

std::shared_ptr<ghost::User> UserManager::getConnectedUser(const std::shared_ptr<ghost::Session>& session) const
{
	if (_connectedUsers.find(session->getUUID()) == _connectedUsers.end()) return nullptr;
	return _connectedUsers.at(session->getUUID());
}

void UserManager::setConnectedUserCallback(std::function<void(std::shared_ptr<ghost::User>)> callback,
					   const std::shared_ptr<ghost::Session>& session)
{
	_connectedUserCallbacks[session->getUUID()] = callback;
}

std::vector<std::shared_ptr<ghost::UserGroup>> UserManager::getUserGroups() const
{
	return std::vector<std::shared_ptr<ghost::UserGroup>>(_groups.begin(), _groups.end());
}

bool UserManager::isUserAlreadyConnected(const std::shared_ptr<User>& user) const
{
	for (const auto& connectedUser : _connectedUsers)
	{
		if (connectedUser.second->getName() == user->getName()) return true;
	}
	return false;
}
