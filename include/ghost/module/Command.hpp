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

#ifndef GHOST_COMMAND_HPP
#define GHOST_COMMAND_HPP

#include <ghost/module/CommandExecutionContext.hpp>
#include <ghost/module/CommandLine.hpp>
#include <ghost/module/CommandParameter.hpp>
#include <list>

namespace ghost
{
/**
 *	Base class for commands executable by the interpreter.
 *	A command has a shortcut, which is used to invoke it, and an execute method which
 *	will be called by the interpreter when a corresponding command line is submitted.
 */
class Command
{
public:
	virtual ~Command() = default;

	/**
	 *	Execution of this command with the provided command line as parameters.
	 *	The method should return true on success, and false otherwise.
	 *	@param commandLine	the parameters for the execution of this command.
	 *	@param context		Container of a ghost::Session and optionally a ghost::Console, representing
	 *		the user who commanded the execution.
	 *	@return true on a successful execution, false otherwise.
	 */
	virtual bool execute(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context) = 0;
	/**
	 *	@return the name of this command.
	 */
	virtual std::string getName() const = 0;
	/**
	 *	The shortcut is the actual string that the user must input in order to invoke
	 *	this command.
	 *	@return the shortcut string of this command.
	 */
	virtual std::string getShortcut() const = 0;
	/**
	 *	Provides a description for this command, which will be displayed to the user if
	 *	the help command is invoked.
	 *	@return the description string of this command.
	 */
	virtual std::string getDescription() const = 0;
	/**
	 *	Optionally implement this function to determine a category that will be used to sort the commands.
	 *	Per default, the category is an empty string.
	 *	@return the category of this command.
	 */
	virtual std::string getCategory() const;
	/**
	 *	Optionally implement this function to generate a sanity check for this command.
	 *	Based on this information, user input is validated before the execution of the command
	 *	and a help page is generated.
	 *	If the expected parameters are not present, an error message is shown to the user.
	 *	@return a list containing required parameters
	 */
	virtual std::list<ghost::CommandParameter> getRequiredParameters() const;
	/**
	 *	Optionally implement this function to generate a sanity check for this command.
	 *	Based on this information, user input is validated before the execution of the command
	 *	and a help page is generated.
	 *	@return a list containing optional parameters
	 */
	virtual std::list<ghost::CommandParameter> getOptionalParameters() const;

private:
	std::map<std::string, ghost::CommandParameter> _requiredParameters;
	std::map<std::string, ghost::CommandParameter> _optionalParameters;
};
} // namespace ghost

#endif // GHOST_COMMAND_HPP
