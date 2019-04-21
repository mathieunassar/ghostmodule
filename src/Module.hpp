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

#ifndef GHOST_INTERNAL_MODULE_HPP
#define GHOST_INTERNAL_MODULE_HPP

#include "Console.hpp"
#include "UserManager.hpp"
#include "CommandLineInterpreter.hpp"
#include <ghost/module/Logger.hpp>

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
			std::shared_ptr<ghost::Console> getConsole();

			/* Logger access */
			void setLogger(const std::shared_ptr<ghost::Logger>& logger);
			std::shared_ptr<ghost::Logger> getLogger() const;

			/* Command line interpretor access */
			std::shared_ptr<ghost::CommandLineInterpreter> getInterpreter();

			/* User manager access */
			std::shared_ptr<ghost::UserManager> getUserManager();

			void printGhostASCII(const std::string& suffix = "");

		private:
			std::string _name;
			State _state;
			std::shared_ptr<Console> _console;
			std::shared_ptr<ghost::Logger> _logger;
			std::shared_ptr<UserManager> _userManager;
			std::shared_ptr<CommandLineInterpreter> _interpreter;
		};
	}
}

#endif // GHOST_MODULE_HPP
