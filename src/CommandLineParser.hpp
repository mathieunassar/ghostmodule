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

#ifndef GHOST_INTERNAL_COMMANDLINEPARSER_HPP
#define GHOST_INTERNAL_COMMANDLINEPARSER_HPP

#include <string>
#include <vector>

#include <ghost/module/CommandLine.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		 *	Parses a line and extracts a command name and a list of parameters.
		 *	Outputs the result as an object of type {@ref CommandLine}
		 */
		class CommandLineParser
		{
		public:
			CommandLine parseCommandLine(const std::string& commandLine);
			CommandLine parseCommandLine(int argc, char* argv[]);

		private:
			template<typename ContainerType>
			CommandLine parseFromContainer(int size, ContainerType container);
			void addParameter(std::map<std::string, std::string>& params, const std::string& name, const std::string& value);

			static void split(std::vector<std::string>& tokens, const std::string& str, const std::string& del);
			static bool isParameterName(const std::string& str, std::string& name);

			int _unknownParametersCount;
		};

		// template definition //

		template<typename ContainerType>
		CommandLine CommandLineParser::parseFromContainer(int size, ContainerType container)
		{
			_unknownParametersCount = 0;

			std::map<std::string, std::string> params;
			std::string commandName = container[0]; // since commandLine is not empty, this is guaranteed to exist

			std::string lastParameterName;
			for (size_t i = 1; i < size; ++i)
			{
				std::string parameterName;
				bool isNameOfAParameter = isParameterName(container[i], parameterName);
				if (isNameOfAParameter) // we are dealing with a new parameter
				{
					if (!lastParameterName.empty()) // save the previous if there is one, this can only occur if previous had no value
					{
						addParameter(params, lastParameterName, "");
					}

					// reset values
					lastParameterName = parameterName;
				}
				else // we are dealing with a value, enter it and reset the values
				{
					addParameter(params, lastParameterName, container[i]);

					// reset the values
					lastParameterName.clear();
				}
			}

			if (!lastParameterName.empty()) // the last split element was a parameter
				addParameter(params, lastParameterName, "");

			return ghost::CommandLine(commandName, params);
		}
	}
}

#endif // GHOST_INTERNAL_COMMANDLINEPARSER_HPP
