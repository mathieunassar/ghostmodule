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

#include <string>
#include <vector>

#include <ghost/module/User.hpp>
#include <ghost/module/PermissionEntity.hpp>

namespace ghost
{
	/**
	 *	Represents a group of users which share permissions.
	 */
	class UserGroup : public PermissionEntity
	{
	public:
		/// @return the name of the group
		virtual const std::string& getName() const = 0;
		/// @return a vector containing all the users of this group
		virtual std::vector<std::shared_ptr<User>> getUsers() const = 0;
		/// @return true if the given user belongs to the group, false otherwise
		virtual bool containsUser(const User& user) const = 0;
	};
}

#endif // GHOST_USERGROUP_HPP
