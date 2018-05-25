#include "../include/internal/UserGroup.hpp"

using namespace ghost::internal;

UserGroup::UserGroup(const std::string& name)
	: _name(name)
{

}

void UserGroup::addUser(std::shared_ptr<User> user)
{
	_users.push_back(user);
}

const std::string& UserGroup::getName() const
{
	return _name;
}

std::vector<std::shared_ptr<ghost::User>> UserGroup::getUsers() const
{
	return std::vector<std::shared_ptr<ghost::User>>(_users.begin(), _users.end());
}

bool UserGroup::containsUser(const ghost::User& user) const
{
	for (auto& usr : _users)
	{
		if (usr->getName() == user.getName())
			return true;
	}
	return false;
}

bool UserGroup::contains(const PermissionEntity& other) const
{
	const UserGroup* cast = dynamic_cast<const UserGroup*>(&other);
	if (cast != nullptr)
		return cast->getName() == _name;
	else
	{
		const User* cast2 = dynamic_cast<const User*>(&other);
		if (cast2 != nullptr)
			return containsUser(*cast2);
	}
	return false;
}
