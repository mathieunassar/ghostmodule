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

#ifndef GHOST_INTERNAL_USER_HPP
#define GHOST_INTERNAL_USER_HPP

#include <ghost/module/User.hpp>
#include <string>

namespace ghost
{
namespace internal
{
class User : public ghost::User
{
public:
	User(const std::string& name, const std::string& password);

	const std::string& getName() const override;

	bool contains(const PermissionEntity& other) const override;

	bool isPasswordCorrect(const std::string& password) const;

private:
	std::string _name;
	std::string _password;
};
} // namespace internal

} // namespace ghost

#endif // GHOST_INTERNAL_USER_HPP
