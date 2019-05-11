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

#ifndef GHOST_COMMANDLINE_HPP
#define GHOST_COMMANDLINE_HPP

#include <string>
#include <sstream>
#include <exception>
#include <map>

namespace ghost
{
	/**
	 *	This class represents a set of parameters used as an input for a command.
	 *	The name of the command can be retrieved with "getCommandName".
	 *	Existence of parameters should be checked with "hasParameter" before they are queried.
	 */
	class CommandLine
	{
	public:
		CommandLine(const std::string& commandName, std::map<std::string, std::string>& parameters);

		const std::string& getCommandName() const;
		bool hasParameter(const std::string& parameterName) const;

		template<typename T>
		T getParameter(const std::string& parameterName) const
		{
			// sanity check, the user should first check if this object has the parameter with "hasParameter"
			if (_parameters.count(parameterName) == 0)
				throw std::invalid_argument("No parameter matches the provided name");

			std::istringstream iss(_parameters.at(parameterName));
			T val;
			iss >> val; // try to cast it
			return val;
		}

		const std::map<std::string, std::string>& getParametersMap() const;

	private:
		std::string _commandName;
		std::map<std::string, std::string> _parameters;
	};
}

#endif // GHOST_COMMANDLINE_HPP
