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

#ifndef GHOST_COMMAND_HPP
#define GHOST_COMMAND_HPP

#include <ghost/module/CommandLine.hpp>

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
		 *	@return true on a successful execution, false otherwise.
		 */
		virtual bool execute(const CommandLine& commandLine) = 0;
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
	};
}

#endif // GHOST_COMMAND_HPP
