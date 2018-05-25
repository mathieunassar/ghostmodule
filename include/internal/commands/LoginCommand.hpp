#ifndef GHOST_INTERNAL_COMMANDS_LOGINCOMMAND_HPP
#define GHOST_INTERNAL_COMMANDS_LOGINCOMMAND_HPP

#include "../../Command.hpp"
#include "../../UserManager.hpp"

namespace ghost
{
	namespace internal
	{
		class LoginCommand : public ghost::Command
		{
		public:
			static const std::string NAME;

			LoginCommand(std::shared_ptr<ghost::UserManager> userManager);

			bool execute(const CommandLine& commandLine) override;
			
			std::string getName() const override;
			std::string getShortcut() const override;
			std::string getDescription() const override;

		private:
			static const std::string _SHORTCUT;
			static const std::string _DESCRIPTION;
			static const std::string _PARAM_USERNAME;
			static const std::string _PARAM_PASSWORD;
			static const std::string _PARAM_USERNAME_SHORT;
			static const std::string _PARAM_PASSWORD_SHORT;

			std::shared_ptr<ghost::UserManager> _userManager;
		};
	}
	
}

#endif // GHOST_INTERNAL_COMMANDS_LOGINCOMMAND_HPP
