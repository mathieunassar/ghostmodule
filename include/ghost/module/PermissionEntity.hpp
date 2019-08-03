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
		virtual ~PermissionEntity() = default;
		/// used to determine whether the permissions of an entity ("other") is contained in this
		virtual bool contains(const PermissionEntity& other) const = 0;
	};
}

#endif // GHOST_PERMISSIONENTITY_HPP
