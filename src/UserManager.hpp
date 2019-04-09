#ifndef GHOST_INTERNAL_USERMANAGER_HPP
#define GHOST_INTERNAL_USERMANAGER_HPP

#include <vector>

#include "User.hpp"
#include "UserGroup.hpp"
#include <ghost/module/UserManager.hpp>

namespace ghost
{
	namespace internal
	{
		class UserManager : public ghost::UserManager
		{
		public:
			UserManager();

			bool createUser(const std::string& name, const std::string& password) override;
			bool createUserGroup(const std::string& name) override;
			void addUserToGroup(const ghost::User& user, ghost::UserGroup& group) override;

			bool connect(const std::string& username, const std::string& password) override;
			void disconnect() override;
			bool isUserConnected() const override;
			User& getConnectedUser() const override;
			void setConnectedUserCallback(std::function<void(const ghost::User&)> callback) override;

			std::vector<std::shared_ptr<ghost::UserGroup>> getUserGroups() const override;

		private:
			std::vector<std::shared_ptr<User>> _users;
			std::vector<std::shared_ptr<UserGroup>> _groups;
			std::shared_ptr<User> _connectedUser;
			std::function<void(const User&)> _connectedUserCallback;
		};
	}
}

#endif // GHOST_INTERNAL_USERMANAGER_HPP
