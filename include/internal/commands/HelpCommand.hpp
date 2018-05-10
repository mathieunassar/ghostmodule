#ifndef GHOST_INTERNAL_COMMANDS_HELPCOMMAND_HPP
#define GHOST_INTERNAL_COMMANDS_HELPCOMMAND_HPP

#include "../../Command.hpp"

namespace Ghost
{
	namespace internal
	{
		class CommandLineInterpreter;

		class HelpCommand : public Ghost::Command
		{
		public:
			static const std::string NAME;

			HelpCommand(CommandLineInterpreter* interpreter);

			bool execute(const CommandLine& commandLine) override;
			
			std::string getName() const override;
			std::string getShortcut() const override;
			std::string getDescription() const override;

		private:
			static const std::string _SHORTCUT;
			static const std::string _DESCRIPTION;

			CommandLineInterpreter* _interpreter;
		};
	}
	
}

#endif // GHOST_INTERNAL_COMMANDS_HELPCOMMAND_HPP
