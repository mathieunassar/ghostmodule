#ifndef GHOST_INTERNAL_USERMANAGER_HPP
#define GHOST_INTERNAL_USERMANAGER_HPP

#include <vector>

#include "User.hpp"
#include "UserGroup.hpp"
#include "../UserManager.hpp"

namespace Ghost
{
	namespace internal
	{
		class UserManager : public Ghost::UserManager
		{
		public:
			UserManager();

			bool createUser(const std::string& name, const std::string& password) override;
			bool createUserGroup(const std::string& name) override;
			void addUserToGroup(const Ghost::User& user, Ghost::UserGroup& group) override;

			bool connect(const std::string& username, const std::string& password) override;
			void disconnect() override;
			bool isUserConnected() const override;
			User& getConnectedUser() const override;
			void setConnectedUserCallback(std::function<void(const Ghost::User&)> callback) override;

			std::vector<std::shared_ptr<Ghost::UserGroup>> getUserGroups() const override;

		private:
			std::vector<std::shared_ptr<User>> _users;
			std::vector<std::shared_ptr<UserGroup>> _groups;
			std::shared_ptr<User> _connectedUser;
			std::function<void(const User&)> _connectedUserCallback;
		};
	}
}

#endif // GHOST_INTERNAL_USERMANAGER_HPP
