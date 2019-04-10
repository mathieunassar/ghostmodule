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
	class Command
	{
	public:
		virtual ~Command() = 0;

		/// received a command line matching its name containing paramters
		virtual bool execute(const CommandLine& commandLine) = 0;
		/// @return the name of this command
		virtual std::string getName() const = 0;
		/// @return the shortcut of the command used to invoke it
		virtual std::string getShortcut() const = 0;
		/// @return a description of the command
		virtual std::string getDescription() const = 0;
	};

	inline Command::~Command() {}
}

#endif // GHOST_COMMAND_HPP
