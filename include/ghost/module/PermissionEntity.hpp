#ifndef GHOST_PERMISSIONENTITY_HPP
#define GHOST_PERMISSIONENTITY_HPP

namespace ghost
{
	/**
	*	Represents a user or a group, or anything that has access rights.
	*/
	class PermissionEntity
	{
	public:
		/// used to determine whether the permissions of an entity ("other") is contained in this
		virtual bool contains(const PermissionEntity& other) const = 0;
	};
}

#endif // GHOST_PERMISSIONENTITY_HPP
