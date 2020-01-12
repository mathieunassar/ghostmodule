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

#ifndef GHOST_INTERNAL_COMMANDLINEINTERPRETER_HPP
#define GHOST_INTERNAL_COMMANDLINEINTERPRETER_HPP

#include <ghost/module/CommandLineInterpreter.hpp>
#include <list>
#include <map>
#include <memory>

#include "UserManager.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Implementation of {@ref ghost::CommandLineInterpreter}.
 */
class CommandLineInterpreter : public ghost::CommandLineInterpreter
{
public:
	CommandLineInterpreter(std::shared_ptr<ghost::UserManager> userManager);

	bool execute(const std::string& commandLine, const ghost::CommandExecutionContext& context) override;
	bool execute(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context) override;

	void registerCommand(std::shared_ptr<Command> command,
			     const std::list<std::shared_ptr<ghost::PermissionEntity>>& permissions = {}) override;

	void printHelp(std::ostream& stream, const std::shared_ptr<ghost::Session>& session) const override;

private:
	struct CommandEntry
	{
		std::shared_ptr<Command> command;
		std::list<std::shared_ptr<PermissionEntity>> permissions;
	};

	bool executionPermitted(const CommandEntry& entry, const std::shared_ptr<ghost::Session>& session) const;

	std::map<std::string, CommandEntry> _commands;
	std::shared_ptr<ghost::UserManager> _userManager;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_COMMANDLINEINTERPRETER_HPP
