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

#include "UserGroup.hpp"

using namespace ghost::internal;

UserGroup::UserGroup(const std::string& name) : _name(name)
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
		if (usr->getName() == user.getName()) return true;
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
		if (cast2 != nullptr) return containsUser(*cast2);
	}
	return false;
}
