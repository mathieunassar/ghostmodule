/*
 * Copyright 2020 Mathieu Nassar
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

#include "User.hpp"

using namespace ghost::internal;

User::User(const std::string& name, const std::string& password) : _name(name), _password(password)
{
}

const std::string& User::getName() const
{
	return _name;
}

bool User::contains(const PermissionEntity& other) const
{
	const User* cast = dynamic_cast<const User*>(&other);
	if (cast != nullptr)
	{
		return cast->getName() == _name;
	}
	return false;
}

bool User::isPasswordCorrect(const std::string& password) const
{
	return _password == password;
}
