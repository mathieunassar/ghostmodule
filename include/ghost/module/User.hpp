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

#ifndef GHOST_USER_HPP
#define GHOST_USER_HPP

#include <ghost/module/PermissionEntity.hpp>
#include <string>

namespace ghost
{
/**
 *	Represents a user of the program.
 *	Possesses a name and is associated to permissions.
 */
class User : public PermissionEntity
{
public:
	virtual ~User() = default;
	/**
	 *	@return the name of this user.
	 */
	virtual const std::string& getName() const = 0;
};
} // namespace ghost

#endif // GHOST_USER_HPP
