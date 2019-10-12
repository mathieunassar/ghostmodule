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

#ifndef GHOST_INTERNAL_COMMANDEXECUTIONCONTEXT_HPP
#define GHOST_INTERNAL_COMMANDEXECUTIONCONTEXT_HPP

#include <ghost/module/CommandExecutionContext.hpp>

namespace ghost
{
namespace internal
{
/**
 *	Context for the execution of a ghost::Command within the ghost::CommandLineInterpreter.
 *	This object contains information usable by the ghost::Command such as a ghost::Session
 *	that can be used to differentiate the users of the module.
 */
class CommandExecutionContext : public ghost::CommandExecutionContext
{
public:
	CommandExecutionContext();

	void setSession(const std::shared_ptr<ghost::Session>& session);
	void setConsole(const std::shared_ptr<ghost::Console>& console);

	const std::shared_ptr<ghost::Session>& getSession() const override;
	std::shared_ptr<ghost::Console> getConsole() const override;

private:
	std::shared_ptr<ghost::Session> _session;
	std::shared_ptr<ghost::Console> _console;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_COMMANDEXECUTIONCONTEXT_HPP
