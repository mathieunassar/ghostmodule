#ifndef GHOST_INTERNAL_COMMANDLINEPARSER_HPP
#define GHOST_INTERNAL_COMMANDLINEPARSER_HPP

#include <string>
#include <vector>

#include "../CommandLine.hpp"

namespace Ghost
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
