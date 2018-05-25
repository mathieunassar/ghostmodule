#ifndef GHOST_USER_HPP
#define GHOST_USER_HPP

#include <string>

#include "PermissionEntity.hpp"

namespace ghost
{
	/**
	 *	Represents a user of the application.
	 *	Possesses a name and is associated to permissions.
	 */
	class User : public PermissionEntity
	{
	public:
		/// @return the name of this user
		virtual const std::string& getName() const = 0;
	};
}

#endif // GHOST_USER_HPP
