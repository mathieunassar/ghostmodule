#include "../include/internal/UserManager.hpp"
#include <exception>

using namespace Ghost::internal;

std::shared_ptr<Ghost::UserManager> Ghost::UserManager::create()
{
	return std::shared_ptr<Ghost::UserManager>(new Ghost::internal::UserManager());
}

UserManager::UserManager()
{
	createUser("superuser", "superuser");
}

void UserManager::addUserToGroup(const Ghost::User& user, Ghost::UserGroup& group)
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

std::vector<std::shared_ptr<Ghost::UserGroup>> UserManager::getUserGroups() const
{
	return std::vector<std::shared_ptr<Ghost::UserGroup>>(_groups.begin(), _groups.end());
}
