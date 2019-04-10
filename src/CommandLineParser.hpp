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

		private:
			static void split(std::vector<std::string>& tokens, const std::string& str, const std::string& del);
			static std::string createParameterName(const std::string prefix, int id);
			static std::string isParameterName(const std::string& str);
			static void addParameter(std::map<std::string, std::string>& params, const std::string& name, const std::string& value);
		};
	}
}

#endif // GHOST_INTERNAL_COMMANDLINEPARSER_HPP
