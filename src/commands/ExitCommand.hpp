#ifndef GHOST_INTERNAL_COMMANDS_EXITCOMMAND_HPP
#define GHOST_INTERNAL_COMMANDS_EXITCOMMAND_HPP

#include <ghost/module/Command.hpp>

namespace ghost
{
	namespace internal
	{
		class Module;

		class ExitCommand : public ghost::Command
		{
		public:
			static const std::string NAME;

			ExitCommand(Module* interpreter);

			bool execute(const CommandLine& commandLine) override;
			
			std::string getName() const override;
			std::string getShortcut() const override;
			std::string getDescription() const override;

		private:
			static const std::string _SHORTCUT;
			static const std::string _DESCRIPTION;

			Module* _module;
		};
	}
	
}

#endif // GHOST_INTERNAL_COMMANDS_EXITCOMMAND_HPP
