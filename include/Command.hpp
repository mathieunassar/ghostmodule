#ifndef GHOST_COMMAND_HPP
#define GHOST_COMMAND_HPP

#include "CommandLine.hpp"

namespace Ghost
{
	class Command
	{
	public:
		/// received a command line matching its name containing paramters
		virtual bool execute(const CommandLine& commandLine) = 0;
		/// @return the name of this command
		virtual std::string getName() const = 0;
		/// @return the shortcut of the command used to invoke it
		virtual std::string getShortcut() const = 0;
		/// @return a description of the command
		virtual std::string getDescription() const = 0;
	};
}

#endif // GHOST_COMMAND_HPP
