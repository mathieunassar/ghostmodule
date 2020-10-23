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

#ifndef GHOST_INTERNAL_USERGROUP_HPP
#define GHOST_INTERNAL_USERGROUP_HPP

#include <ghost/module/UserGroup.hpp>
#include <memory>
#include <string>
#include <vector>

#include "User.hpp"

namespace ghost
{
namespace internal
{
class UserGroup : public ghost::UserGroup
{
public:
	UserGroup(const std::string& name);

	void addUser(std::shared_ptr<User> user);

	const std::string& getName() const override;
	std::vector<std::shared_ptr<ghost::User>> getUsers() const override;
	bool containsUser(const ghost::User& user) const override;

	bool contains(const PermissionEntity& other) const override;

private:
	std::string _name;
	std::vector<std::shared_ptr<User>> _users;
};
} // namespace internal

} // namespace ghost

#endif // GHOST_INTERNAL_USERGROUP_HPP
