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

#ifndef GHOST_USERMANAGER_HPP
#define GHOST_USERMANAGER_HPP

#include <vector>
#include <memory>
#include <functional>

#include <ghost/module/User.hpp>
#include <ghost/module/UserGroup.hpp>

namespace ghost
{
	class UserManager
	{
	public:
		static std::shared_ptr<UserManager> create();
		virtual ~UserManager() = 0;

		virtual bool createUser(const std::string& name, const std::string& password) = 0;
		virtual bool createUserGroup(const std::string& name) = 0;
		virtual void addUserToGroup(const User& user, UserGroup& group) = 0;

		virtual bool connect(const std::string& username, const std::string& password) = 0;
		virtual void disconnect() = 0;
		virtual bool isUserConnected() const = 0;
		virtual User& getConnectedUser() const = 0;
		virtual void setConnectedUserCallback(std::function<void(const User&)> callback) = 0;
		
		virtual std::vector<std::shared_ptr<ghost::UserGroup>> getUserGroups() const = 0;
	};

	inline UserManager::~UserManager() {}
}

#endif // GHOST_USERMANAGER_HPP
