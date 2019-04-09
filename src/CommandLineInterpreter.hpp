#ifndef GHOST_INTERNAL_COMMANDLINEINTERPRETER_HPP
#define GHOST_INTERNAL_COMMANDLINEINTERPRETER_HPP

#include <list>
#include <map>
#include <memory>

#include <ghost/module/CommandLineInterpreter.hpp>
#include "UserManager.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		 *	Implementation of {@ref ghost::CommandLineInterpreter}.
		 */
		class CommandLineInterpreter : public ghost::CommandLineInterpreter
		{
		public:
			CommandLineInterpreter(std::shared_ptr<ghost::UserManager> userManager);

			bool execute(const std::string& commandLine) override;

			void registerCommand(std::shared_ptr<Command> command, const std::list<std::shared_ptr<ghost::PermissionEntity>>& permissions = {}) override;

			void printHelp(std::ostream& stream) const override;

		private:
			struct CommandEntry
			{
				std::shared_ptr<Command> command;
				std::list<std::shared_ptr<PermissionEntity>> permissions;
			};

			bool executionPermitted(const CommandEntry& entry) const;

			std::map<std::string, CommandEntry> _commands;
			std::shared_ptr<ghost::UserManager> _userManager;
		};
	}
}

#endif // GHOST_INTERNAL_COMMANDLINEINTERPRETER_HPP
