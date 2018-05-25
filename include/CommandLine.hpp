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

	private:
		std::string _commandName;
		std::map<std::string, std::string> _parameters;
	};
}

#endif // GHOST_COMMANDLINE_HPP
