/*
 * Copyright 2020 Mathieu Nassar
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

#ifndef GHOST_MODULE_BUILDER_HPP
#define GHOST_MODULE_BUILDER_HPP

#include <functional>
#include <ghost/module/Console.hpp>
#include <ghost/module/Logger.hpp>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleExtensionBuilder.hpp>
#include <ghost/module/ThreadPool.hpp>
#include <memory>
#include <string>

namespace ghost
{
/**
 *	The module builder allows the developer to set up the different components
 *	of the module before it is started.
 */
class ModuleBuilder
{
public:
	static std::unique_ptr<ghost::ModuleBuilder> create();

	virtual ~ModuleBuilder() = default;

	/**
	 *	Sets the function that will be called to initialize the module.
	 *	The function must return true if the initialization was successful, false
	 *	otherwise.
	 *	If this method is not called, the initialization will not perform any
	 *	action and will be considered successful.
	 *	@param behavior	function to be called when the module initializes.
	 *	@return an instance of this.
	 */
	virtual ModuleBuilder& setInitializeBehavior(const std::function<bool(const ghost::Module&)>& behavior) = 0;
	/**
	 *	Sets the function that will be called cyclically during the module's
	 *	runtime. The function must return true if another cycle is necessary (if
	 *	it must be called again), and false if the module can dispose.
	 *	If this method is not called, nothing will happen and the module will
	 *	immediately dispose.
	 *	@param behavior	function to be called cyclically during the module's runtime.
	 *	@return an instance of this.
	 */
	virtual ModuleBuilder& setRunningBehavior(const std::function<bool(const ghost::Module&)>& behavior) = 0;
	/**
	 *	Sets the function that will be called to dispose the module.
	 *	If an error happened during the runtime of the module, this function
	 *	will still be called.
	 *	If this method is not called, nothing will happen at the disposition of
	 *	the module and the program will immediatly exit.
	 *	@param behavior	function to be called when the module disposes.
	 *	@return an instance of this.
	 */
	virtual ModuleBuilder& setDisposeBehavior(const std::function<void(const ghost::Module&)>& behavior) = 0;
	/**
	 *	Passes the program options to the builder. The options will be parsed and
	 *	made available to the built ghost::Module instance.
	 *	@param argc	number of arguments present in "argv"
	 *	@param argv	array of C-style string containing the program options.
	 *	@return an instance of this.
	 */
	virtual ModuleBuilder& setProgramOptions(int argc, char* argv[]) = 0;
	/**
	 *	Retrieves a ghost::ThreadPool that will be used by the module.
	 *	It can be used prior to the module build call to configure module extensions
	 *	or to parameterize the pool's size.
	 *	Per default, the pool will be constructed with as many threads as the hardware
	 *	concurrency limit.
	 *	Note: Threads will only be created after the thread pool is started, which happens
	 *	after the initialization behavior is called.
	 *	The thread pool may optionally also be started before calling "build" (by calling
	 *	"start" on the pool).
	 *	Per default one thread pool is started by the module, with an empty name.
	 *	Users can configured multiple thread pool at module-build time or during runtime.
	 *	@param label the name of the thread pool to return
	 *	@return the thread pool of the module being constructed.
	 */
	virtual std::shared_ptr<ghost::ThreadPool> getThreadPool(const std::string& label = "") const = 0;
	/**
	 *	Adds a ghost::ThreadPool to the module's execution. The pool is returned
	 *	and is not started automatically, so that the pool may be configured before
	 *	it gets started.
	 *	@param label the name of the thread pool to add.
	 *	@param threadsCount the number of threads managed by the thread pool to add.
	 *	@return the thread pool created by the module, stored with the given label.
	 */
	virtual std::shared_ptr<ghost::ThreadPool> addThreadPool(const std::string& label, size_t threadsCount) = 0;
	/**
	 *	Activates the console management. A handle to the created ghost::Console
	 *	is returned for further configuration possibilities.
	 *	@return a handle to a ghost::Console.
	 */
	virtual std::shared_ptr<ghost::Console> setConsole() = 0;
	/**
	 *	Sets the logger of this module.
	 *	@param logger	logger to set.
	 *	@return an instance of this.
	 */
	virtual ModuleBuilder& setLogger(const std::shared_ptr<ghost::Logger>& logger) = 0;
	/**
	 *	Adds a module extension builder to this builder. Its "build" method will be called
	 *	when building this module.
	 *	@param builder	Builder for a module extension.
	 *	@return an instance of this.
	 */
	virtual ModuleBuilder& addExtensionBuilder(const std::shared_ptr<ghost::ModuleExtensionBuilder>& builder) = 0;
	/**
	 *	Builds the module based on the parameters provided to this builder.
	 *	@param moduleName	name of the module to build.
	 *	@return the built module if the process was successful, nullptr otherwise.
	 */
	virtual std::shared_ptr<ghost::Module> build(const std::string& moduleName = "") = 0;

protected:
	static void setModuleToExtension(const std::shared_ptr<ghost::Module>& module,
					 const std::shared_ptr<ghost::ModuleExtension>& component);
};
} // namespace ghost

#endif // GHOST_MODULE_BUILDER_HPP
