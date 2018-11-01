#ifndef GHOST_USERMANAGER_HPP
#define GHOST_USERMANAGER_HPP

#include <vector>
#include <memory>
#include <functional>
#include "User.hpp"
#include "UserGroup.hpp"

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
