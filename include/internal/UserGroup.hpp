#ifndef GHOST_INTERNAL_USERGROUP_HPP
#define GHOST_INTERNAL_USERGROUP_HPP

#include <string>
#include <vector>
#include <memory>

#include "../UserGroup.hpp"
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
	}
	
}

#endif // GHOST_INTERNAL_USERGROUP_HPP
