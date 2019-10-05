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

#ifndef GHOST_USERGROUP_HPP
#define GHOST_USERGROUP_HPP

#include <ghost/module/PermissionEntity.hpp>
#include <ghost/module/User.hpp>
#include <string>
#include <vector>

namespace ghost
{
/**
 *	Represents a group of users which share permissions.
 */
class UserGroup : public PermissionEntity
{
public:
	/**
	 *	@return the name of this group.
	 */
	virtual const std::string& getName() const = 0;
	/**
	 *	@return a list of users in this group.
	 */
	virtual std::vector<std::shared_ptr<User>> getUsers() const = 0;
	/**
	 *	@param user	user to check if they belong to this group.
	 *	@return true if the provided user belongs to this group.
	 */
	virtual bool containsUser(const User& user) const = 0;
};
} // namespace ghost

#endif // GHOST_USERGROUP_HPP
