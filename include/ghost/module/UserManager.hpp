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

#include <functional>
#include <ghost/module/User.hpp>
#include <ghost/module/UserGroup.hpp>
#include <memory>
#include <vector>

namespace ghost
{
/**
 *	The user manager contains information about the user of this program.
 *	Such information may be used to control the access to some features. For example,
 *	the command line interpreter can use the user manager to restrict access to some
 *	commands.
 *	The user manager provides methods to create users and group of users, and to
 *	connect and disconnect a user.
 */
class UserManager
{
public:
	/**
	 *	Creates an instance of the user manager.
	 */
	static std::shared_ptr<UserManager> create();
	virtual ~UserManager() = default;

	/**
	 *	Creates a new user with the provided information. If a user with the provided name
	 *	already exists, then this call fails and returns nullptr.
	 *	@param name	chosen name of the user.
	 *	@param password	password for this user.
	 *	@return the newly created user, if the creation is successful, nullptr otherwise.
	 */
	virtual std::shared_ptr<ghost::User> createUser(const std::string& name, const std::string& password) = 0;
	/**
	 *	Creates a new user group of the provided name. If a group with the provided name
	 *	already exists, then this call fails and returns nullptr.
	 *	@param name	chosen name of the group.
	 *	@return the newly created group of user if the creation is successful, nullptr otherwise.
	 */
	virtual std::shared_ptr<ghost::UserGroup> createUserGroup(const std::string& name) = 0;
	/**
	 *	Adds the provided user to the provided group. The user and the group must be managed
	 *	by this user manager.
	 *	@param user	user to add to the group.
	 *	@param group	group to add the user to.
	 */
	virtual void addUserToGroup(const ghost::User& user, ghost::UserGroup& group) = 0;

	/**
	 *	If a ghost::User matches the provided credentials in this user manager, sets this
	 *	user as "connected", i.e. as active user of this program.
	 *	@param username	name of the user to connect.
	 *	@param password	password of the user to connect.
	 *	@return true if the connection is successful, false otherwise.
	 */
	virtual bool connect(const std::string& username, const std::string& password) = 0;
	/**
	 *	If a ghost::User was connected previously, resets this parameters.
	 *	After this call, ghost::UserManager::isUserConnected returns false.
	 */
	virtual void disconnect() = 0;
	/**
	 *	@return true if ghost::UserManager::connect was successfully called and if
	 *		ghost::UserManager::disconnect was not called afterward.
	 */
	virtual bool isUserConnected() const = 0;
	/**
	 *	@return the connected user if ghost::UserManager::isUserConnected would return true,
	 *		returns nullptr otherwise.
	 */
	virtual std::shared_ptr<ghost::User> getConnectedUser() const = 0;
	/**
	 *	Sets a callback to be notified if a new user connects or if a disconnection happens.
	 *	In case of a user disconnection, the callback is called with nullptr.
	 *	@param callback	callback that will be called after connections or disconnections.
	 */
	virtual void setConnectedUserCallback(std::function<void(std::shared_ptr<ghost::User>)> callback) = 0;

	/**
	 *	@returns the list of user groups existing in this user manager.
	 */
	virtual std::vector<std::shared_ptr<ghost::UserGroup>> getUserGroups() const = 0;
};
} // namespace ghost

#endif // GHOST_USERMANAGER_HPP
