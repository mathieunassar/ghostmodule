#ifndef GHOST_COMMANDLINEINTERPRETER_HPP
#define GHOST_COMMANDLINEINTERPRETER_HPP

#include <iostream>
#include <list>
#include <string>
#include <memory>

#include <ghost/module/PermissionEntity.hpp>

namespace ghost
{
	class Command;
	class UserManager;

	/**
	 *	Entry point to the execution of custom commands using the user input.
	 *	Users of this class can add commands using the "registerCommand" method.
	 *	Calling execute with a line beginning with the name of the command will automatically call
	 *	the matching command. The rest of the line will be parsed into parameters so that:
	 *	- parameters are separated by a space
	 *	- parameter names begin with a dash ('-') or a double dash ('--')
	 *	- a value corresponding to a parameter must immediatly follow the parameter name (separated by a space)
	 *	- if no parameter name precedes a value (without dash), then the value belongs to a parameter with no name
	 *	- parameters can have no value, their presence can still be checked upon in the {@ref CommandLine} class.
	 */
	class CommandLineInterpreter
	{
	public:
		static std::shared_ptr<CommandLineInterpreter> create(std::shared_ptr<UserManager> userManager);
		virtual ~CommandLineInterpreter() = 0;

		/// searches for the corresponding command and executes it with the given parameters
		virtual bool execute(const std::string& commandLine) = 0;
		/// adds a command to the interpreter's registry
		virtual void registerCommand(std::shared_ptr<Command> command, const std::list<std::shared_ptr<PermissionEntity>>& permissions = {}) = 0;
		/// prints the information about all the commands
		virtual void printHelp(std::ostream& stream) const = 0;
	};

	inline CommandLineInterpreter::~CommandLineInterpreter() {}
}

#endif // GHOST_COMMANDLINEINTERPRETER_HPP
