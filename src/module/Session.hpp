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

#ifndef GHOST_INTERNAL_SESSION_HPP
#define GHOST_INTERNAL_SESSION_HPP

#include <ghost/module/Session.hpp>

namespace ghost
{
namespace internal
{
/**
 *	A ghost::Session is an object that can be used to determine which user is issuing a
 *	command or is being logged in. It contains a UUID that can be used to store data for
 *	specific control sessions.
 *	For example, a remote controller (using the ghost_connected_module library) commands
 *	the ghost::CommandLineInterpreter from different clients, and provides a different
 *	session (with a different UUID) for each remote control.
 */
class Session : public ghost::Session
{
public:
	Session(const std::string& uuid);

	const std::string& getUUID() const override;

private:
	std::string _uuid;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_PROMPT_HPP
