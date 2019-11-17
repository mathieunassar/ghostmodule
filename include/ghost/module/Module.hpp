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

#include <ghost/module/CommandLine.hpp>
#include <ghost/module/CommandLineInterpreter.hpp>
#include <ghost/module/Console.hpp>
#include <ghost/module/Logger.hpp>
#include <ghost/module/ModuleComponent.hpp>
#include <ghost/module/UserManager.hpp>
#include <memory>
#include <string>

namespace ghost
{
/**
 *	This class contains the components of the execution of the current program.
 *	It is generated by the builder ghost::ModuleBuilder. Users must call
 *	ghost::Module::start to begin the execution of the module.
 *	The parameterized initialization behavior will first be called. Then, the
 *	function set to be the running behavior will be called cyclically until either
 *	it returns false or an error occurs. Finally, the dispose behavior function is
 *	called to clean-up the program before it gets terminated.
 *	These steps happens during the call to ghost::Module::start (no thread is
 *	generated for that purpose), therefore this call is blocking until the end
 *	of the execution.
 */
class Module
{
public:
	virtual ~Module() = default;

	/**
	 *	Starts the module. The ghost::ModuleExecutor will be called and its
	 *	ghost::ModuleExecutor::initialize, ghost::ModuleExecutor::run and
	 *	ghost::ModuleExecutor::dispose methods will be executed in this order.
	 *	If an error occurs, the execution is interrupted and this method returns.
	 */
	virtual void start() = 0;
	/**
	 *	Triggers the disposition of the module.
	 *	The current running cycle will be executed until its end, and the disposition
	 *	function will be called.
	 *	Note: if ghost::ModuleBuilder::setRunningBehavior was called with a method
	 *	that blocks the execution forever, this call will block (the execution is not
	 *	interrupted during a cycle).
	 */
	virtual void stop() = 0;
	/**
	 *	If ghost::ModuleBuilder::setConsole was called, returns the console handle
	 *	that was generated. Otherwise, returns nullptr.
	 *	The ghost::Console can be used to read lines, set the output mode, the prompt
	 *	format, and other i/o parameters.
	 *	@return the console or nullptr if it was not initalized
	 */
	virtual std::shared_ptr<ghost::Console> getConsole() const = 0;
	/**
	 *	@return The logger used by this module. If ghost::ModuleBuilder::setLogger
	 *	was not called, this call will return an instance of ghost::stdoutLogger.
	 */
	virtual std::shared_ptr<ghost::Logger> getLogger() const = 0;
	/**
	 *	The command line interpreter can be used to register new commands and
	 *	execute registered commands based on a string input.
	 *	If ghost::ModuleBuilder::setConsole was called, user input will be bound
	 *	to the input of the interpreter, i.e user input lines are directly used
	 *	as command lines.
	 *	@return the command line interpreter used by this module.
	 */
	virtual std::shared_ptr<ghost::CommandLineInterpreter> getInterpreter() const = 0;
	/**
	 *	The user manager can be used to add users, groups of users, and is the
	 *	interface used to connect users and to access the connected users of
	 *	this program.
	 *	@return the instance of the UserManager used by this module.
	 */
	virtual std::shared_ptr<ghost::UserManager> getUserManager() const = 0;
	/**
	 *	Gets a map containing the parameters given to the program when starting it.
	 *	The program options must first have been provided to ghost::ModuleBuilder.
	 *	@return a ghost::CommandLine object containing the parsed program options.
	 */
	virtual const ghost::CommandLine& getProgramOptions() const = 0;
	/**
	 *	@returns the name of the module.
	 */
	virtual const std::string& getModuleName() const = 0;
	/**
	 *	@return the component of this module whose type mathces the template parameter.
	 *		If no such component exists in the module, returns nullptr.
	 */
	template <typename ComponentType>
	const std::shared_ptr<ComponentType> getComponent() const
	{
		return std::dynamic_pointer_cast<ComponentType>(getComponent(ComponentType::NAME));
	}
	/**
	 *	Prints "GHOST" in ASCII characters.
	 */
	virtual void printGhostASCII(const std::string& suffix = "") const = 0;

protected:
	/**
	 *	Returns the component of the type with the provided type name.
	 *	@param typeName	name of the type of component.
	 *	@return the component, or nullptr if no such component exists.
	 */
	virtual std::shared_ptr<ghost::ModuleComponent> getComponent(const std::string& typeName) const = 0;
};
} // namespace ghost

#endif // GHOST_MODULE_HPP
