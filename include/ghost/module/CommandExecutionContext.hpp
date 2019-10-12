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

#ifndef GHOST_COMMANDEXECUTIONCONTEXT_HPP
#define GHOST_COMMANDEXECUTIONCONTEXT_HPP

#include <ghost/module/Console.hpp>
#include <ghost/module/Session.hpp>
#include <memory>

namespace ghost
{
/**
 *	Context for the execution of a ghost::Command within the ghost::CommandLineInterpreter.
 *	This object contains information usable by the ghost::Command such as a ghost::Session
 *	that can be used to differentiate the users of the module.
 */
class CommandExecutionContext
{
public:
	/**
	 *	Constructor of the ghost::CommandExecutionContext.
	 *	@param session	the minimum amount of informatio required by the context.
	 */
	CommandExecutionContext(const std::shared_ptr<ghost::Session>& session);
	/**
	 *	Sets the ghost::Console available through this command execution context.
	 *	@param console a ghost::Console implementation.
	 */
	void setConsole(const std::shared_ptr<ghost::Console>& console);
	/**
	 *	@return the session to which the execution of the command is bound.
	 */
	const std::shared_ptr<ghost::Session>& getSession() const;
	/**
	 *	@return a shared_ptr to a ghost::Console, or nullptr if none was configured in the
	 *	module.
	 */
	std::shared_ptr<ghost::Console> getConsole() const;

private:
	std::shared_ptr<ghost::Session> _session;
	std::shared_ptr<ghost::Console> _console;
};
} // namespace ghost

#endif // GHOST_COMMANDEXECUTIONCONTEXT_HPP
