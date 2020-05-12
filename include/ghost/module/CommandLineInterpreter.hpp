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

#ifndef GHOST_COMMANDLINEINTERPRETER_HPP
#define GHOST_COMMANDLINEINTERPRETER_HPP

#include <ghost/module/CommandExecutionContext.hpp>
#include <ghost/module/CommandLine.hpp>
#include <ghost/module/Logger.hpp>
#include <ghost/module/PermissionEntity.hpp>
#include <iostream>
#include <list>
#include <memory>
#include <string>

namespace ghost
{
class Command;
class UserManager;

/**
 *	The command line interpreter is responsible for the execution of command lines. Custom commands
 *	are implemented as ghost::Command objects, which must first be registered to the interpreter.
 *	Commands can then be invoked by either one of the following ways:
 *	- providing a ghost::CommandLine object that will contain the necessary parameters for this command;
 *	- providing a string containing the shortcut of the command, followed by the parameters, separated
 *		by spaces.
 *	If a string is provided to the interpreter, its parameter list will be parsed as follows:
 *	- parameters are separated by a space;
 *	- parameter names begin with a dash ('-') or a double dash ('--');
 *	- a value corresponding to a parameter must immediatly follow the parameter name (separated by a space);
 *	- if no parameter name precedes a value (without dash), then the value belongs to a parameter with no name;
 *	- it is possible to provide a parameter without a value; its presence can still be checked in the
 *		ghost::CommandLine class.
 */
class CommandLineInterpreter
{
public:
	/**
	 *	Creates an instance of the command line interpreter that ignores permission.
	 */
	static std::shared_ptr<CommandLineInterpreter> create(std::shared_ptr<ghost::Logger> logger = {});
	/**
	 *	Creates an instance of the command line interpreter that can manage permissions.
	 */
	static std::shared_ptr<CommandLineInterpreter> create(std::shared_ptr<UserManager> userManager,
							      std::shared_ptr<ghost::Logger> logger = {});
	virtual ~CommandLineInterpreter() = default;

	/**
	 *	Parses the provided string to a ghost::CommandLine. Then, searches for a registered ghost::Command
	 *	whose shortcut matches the command's name. If such a command is found, executes it and returns its
	 *	execution result. Otherwise, returns false.
	 *	@param commandLine	String containing a command's shortcut and parameters separated by spaces.
	 *	@param context		Container of a ghost::Session and optionally a ghost::Console, representing
	 *		the user who commanded the execution.
	 *	@return true if a matching command was found and successfully executed, false otherwise.
	 */
	virtual bool execute(const std::string& commandLine, const ghost::CommandExecutionContext& context) = 0;
	/**
	 *	Searches for a registered ghost::Command whose shortcut matches the command's name. If such a
	 *	command is found, executes it and returns its execution result. Otherwise, returns false.
	 *	@param commandLine	parsed command line containing a command's shortcut and parameters.
	 *	@param context		Container of a ghost::Session and optionally a ghost::Console, representing
	 *		the user who commanded the execution.
	 *	@return true if a matching command was found and successfully executed, false otherwise.
	 */
	virtual bool execute(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context) = 0;
	/**
	 *	Adds a command to the list of known executable commands. If a command is already registered with
	 *	the same name, the older command will be replaced with the new one.
	 *	It is possible to provide a list of ghost::PermissionEntity to restrict the access of the new
	 *	command to selected users. If the provided list is empty, then the command is available to everyone.
	 *	If ghost::CommandLineInterpreter::create was called without the user manager, then permissions will
	 *	be ignored by the interpreter.
	 *	@param command	the command to register.
	 *	@param permissions	list of users or group of users allowed to use this command.
	 */
	virtual void registerCommand(std::shared_ptr<ghost::Command> command,
				     const std::list<std::shared_ptr<ghost::PermissionEntity>>& permissions = {}) = 0;
	/**
	 *	Prints the name, shortcut and description of all the registered commands into the provided stream.
	 *	@param stream	stream in which the help will be printed.
	 *	@param session	the session of the current execution used to determine are available to the current
	 *		operator.
	 */
	virtual void printHelp(std::ostream& stream, const std::shared_ptr<ghost::Session>& session) const = 0;

	/**
	 *	Prints the name, shortcut, description as well as all registered required and optional parameters of
	 *	the command referenced by "commandName".
	 *	@param stream	stream in which the help will be printed.
	 *	@param commandName	name of the command to print the help of.
	 *	@param session	the session of the current execution used to determine are available to the current
	 *		operator.
	 *	@return true if the corresponding command was found, false otherwise.
	 */
	virtual bool printCommandHelp(std::ostream& stream, const std::string& commandName,
				      const std::shared_ptr<ghost::Session>& session) const = 0;
};

} // namespace ghost

#endif // GHOST_COMMANDLINEINTERPRETER_HPP
