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
