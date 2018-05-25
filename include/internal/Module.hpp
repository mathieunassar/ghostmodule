#ifndef GHOST_INTERNAL_MODULE_HPP
#define GHOST_INTERNAL_MODULE_HPP

#include "Console.hpp"
#include "UserManager.hpp"
#include "CommandLineInterpreter.hpp"

namespace ghost
{
	namespace internal
	{
		class Module
		{
		public:
			enum State
			{
				STOPPED,		// deinitialized
				INITIALIZING,	// init method is being called
				RUNNING,		// run method is being called
				DISPOSING		// dispose method is being called
			};

			Module(const std::string& name = "");
			~Module();

			bool setState(State state);
			State getState() const;

			/* Console access */
			void initializeConsole();
			bool hasConsole() const;
			std::shared_ptr<ghost::Console> getConsole();

			/* Command line interpretor access */
			std::shared_ptr<ghost::CommandLineInterpreter> getInterpreter();

			/* User manager access */
			std::shared_ptr<ghost::UserManager> getUserManager();

		private:
			std::string _name;
			State _state;
			std::shared_ptr<Console> _console;
			std::shared_ptr<UserManager> _userManager;
			std::shared_ptr<CommandLineInterpreter> _interpreter;
		};
	}
}

#endif // GHOST_MODULE_HPP
