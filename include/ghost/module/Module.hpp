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

#ifndef GHOST_MODULE_HPP
#define GHOST_MODULE_HPP

#include <memory>

#include <ghost/module/Console.hpp>
#include <ghost/module/UserManager.hpp>
#include <ghost/module/CommandLineInterpreter.hpp>
#include <ghost/module/Logger.hpp>

namespace ghost
{
	namespace internal
	{
		class Module;
	}

	/**
	 * @brief Base class for the Module functionality.
	 * 
	 * Implementations of this interface will be executed after a call to
	 * the method "start". The execution order is as follows:
	 * - init()
	 * - run()
	 * - dispose()
	 * 
	 * The class provides an access to the console mechanism (see Console),
	 * which allows the developer to manage input and outputs in the program.
	 * 
	 * A command line interpreter can also be accessed and provides the
	 * possibility to create commands that the user can use (See 
	 * CommandLineInterpreter).
	 * 
	 * Finally, the program can control access to its features by controlling
	 * which user currently uses the program, through the UserManager instance.
	 */
	class Module
	{
	public:
		/**
		 * @brief Constructor of the Module object
		 * 
		 * @param name name of the module
		 */
		Module(const std::string& name = "");

		/**
		 * @brief Starts the execution of the module.
		 * 
		 * In the following order, init, run and dispose will be called.
		 */
		void start();

		/**
		 * @brief Helper method to pause the execution of the current thread for
		 * a given time.
		 * 
		 * @param ms time to sleep
		 */
		static void sleepMillisecond(int ms);

	protected:
		/**
		 * @brief Perform initialization tasks during the call to this method.
		 * 
		 * The return value can be used to tell the Module mechanism that the
		 * intialization sequence failed. In such a case, the run method will not be
		 * called, and dispose will be directly called.
		 * 
		 * @return true if the initalization succeeded
		 * @return false if the initialization failed.
		 */
		virtual bool init();

		/**
		 * @brief Runs a cycle of the module.
		 * 
		 * This method will be called in a loop, as long as its return value is true.
		 * Run is called after a successful initialization and before dispose().
		 * 
		 * @return true if run should be called again
		 * @return false if the module finished and dispose can be called
		 */
		virtual bool run();

		/**
		 * @brief Cleans the state of the module before it terminates.
		 * 
		 * During this call, clean up operations (such as closing connections for example)
		 * can be executed.
		 * At the end of this call, the module will terminate.
		 */
		virtual void dispose();

		/**
		 * @brief Initializes the Console interface which can afterwards be accessed
		 * by getConsole.
		 */
		void initializeConsole();

		/**
		 * @brief Get the Console interface if it was initialized with "initializeConsole".
		 * 
		 * @return the console or nullptr if it was not initalized
		 */
		std::shared_ptr<Console> getConsole();

		/** 
		 *	Sets the logger of this module.
		 *	@param logger	logger to set.
		 */
		void setLogger(const std::shared_ptr<Logger>& logger);

		/**
		 *	@return the logger previously set, or nullptr.
		 */
		std::shared_ptr<Logger> getLogger() const;

		/**
		 * @brief gets the command line executor.
		 * 
		 * The command line interpreter can be used to register new commands and execute registered
		 * commands based on a string input.
		 * 
		 * Calling "initializeConsole" will bind the user input to the input of the interpreter,
		 * i.e. the user can trigger commands by writing in the process window.
		 * 
		 * @return std::shared_ptr<CommandLineInterpreter> 
		 */
		std::shared_ptr<CommandLineInterpreter> getInterpreter();

		/**
		 * @brief gets the user manager.
		 * 
		 * The user manager can be used to add users, groups of users, and is the interface
		 * used to connect users and to access the connected users.
		 * 
		 * @return the instance of the UserManager.
		 */
		std::shared_ptr<UserManager> getUserManager();

		/**
		 * @brief Prints "GHOST" in ASCII characters.
		 */
		void printGhostASCII(const std::string& suffix = "");

	private:
		std::shared_ptr<ghost::internal::Module> _internal;
	};
}

#endif // GHOST_MODULE_HPP
