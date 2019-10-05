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

#ifndef GHOST_INTERNAL_USERMANAGER_HPP
#define GHOST_INTERNAL_USERMANAGER_HPP

#include <ghost/module/UserManager.hpp>
#include <vector>

#include "User.hpp"
#include "UserGroup.hpp"

namespace ghost
{
namespace internal
{
class UserManager : public ghost::UserManager
{
public:
	UserManager();

	std::shared_ptr<ghost::User> createUser(const std::string& name, const std::string& password) override;
	std::shared_ptr<ghost::UserGroup> createUserGroup(const std::string& name) override;
	void addUserToGroup(const ghost::User& user, ghost::UserGroup& group) override;

	bool connect(const std::string& username, const std::string& password) override;
	void disconnect() override;
	bool isUserConnected() const override;
	std::shared_ptr<ghost::User> getConnectedUser() const override;
	void setConnectedUserCallback(std::function<void(std::shared_ptr<ghost::User>)> callback) override;

	std::vector<std::shared_ptr<ghost::UserGroup>> getUserGroups() const override;

private:
	std::vector<std::shared_ptr<User>> _users;
	std::vector<std::shared_ptr<UserGroup>> _groups;
	std::shared_ptr<User> _connectedUser;
	std::function<void(std::shared_ptr<ghost::User>)> _connectedUserCallback;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_USERMANAGER_HPP
