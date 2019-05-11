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
			static CommandLine parseCommandLine(const std::string& commandLine);
			static CommandLine parseCommandLine(int argc, char* argv[]);

		private:
			static void split(std::vector<std::string>& tokens, const std::string& str, const std::string& del);
			static std::string createParameterName(const std::string prefix, int id);
			static std::string isParameterName(const std::string& str);
			static void addParameter(std::map<std::string, std::string>& params, const std::string& name, const std::string& value);

			template<typename ContainerType>
			static CommandLine parseFromContainer(int size, ContainerType container);
		};

		// template definition //

		template<typename ContainerType>
		CommandLine CommandLineParser::parseFromContainer(int size, ContainerType container)
		{
			std::map<std::string, std::string> params;
			std::string commandName = container[0]; // since commandLine is not empty, this is guaranteed to exist

			std::string tmpParameterName;
			for (size_t i = 1; i < size; ++i)
			{
				std::string isParameterNameRes = isParameterName(container[i]);
				if (!isParameterNameRes.empty()) // we are dealing with a new parameter
				{
					if (!tmpParameterName.empty()) // save the previous if there is one, this can only occur if previous had no value
					{
						addParameter(params, tmpParameterName, "");
					}

					// reset values
					tmpParameterName = isParameterNameRes;
				}
				else // we are dealing with a value, enter it and reset the values
				{
					if (tmpParameterName.empty()) // value has no parameter name
						tmpParameterName = "undefined";

					addParameter(params, tmpParameterName, container[i]);

					// reset the values
					tmpParameterName = "";
				}
			}

			if (!tmpParameterName.empty()) // the last split element was a parameter
				addParameter(params, tmpParameterName, "");

			return ghost::CommandLine(commandName, params);
		}
	}
}

#endif // GHOST_INTERNAL_COMMANDLINEPARSER_HPP
