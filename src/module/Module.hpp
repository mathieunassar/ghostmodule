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

#include <condition_variable>
#include <ghost/module/Module.hpp>
#include <mutex>
#include <thread>

#include "CommandLineInterpreter.hpp"
#include "Console.hpp"
#include "ThreadPool.hpp"
#include "UserManager.hpp"

namespace ghost
{
namespace internal
{
class Module : public ghost::Module
{
public:
	enum State
	{
		STOPPED,      // deinitialized
		INITIALIZING, // init method is being called
		RUNNING,      // run method is being called
		DISPOSING     // dispose method is being called
	};

	Module(const std::string& name, const std::shared_ptr<ThreadPool>& threadPool,
	       const std::shared_ptr<Console>& console, const std::shared_ptr<ghost::Logger>& logger,
	       const ghost::CommandLine& options,
	       const std::vector<std::shared_ptr<ghost::ModuleExtension>>& components,
	       const std::function<bool(const ghost::Module&)>& initializationBehavior,
	       const std::function<bool(const ghost::Module&)>& runningBehavior,
	       const std::function<void(const ghost::Module&)>& disposeBehavior);
	~Module();

	bool setState(State state);
	State getState() const;

	// From ghost::Module
	void start() override;
	void stop() override;

	std::shared_ptr<ghost::Console> getConsole() const override;
	std::shared_ptr<ghost::Logger> getLogger() const override;
	std::shared_ptr<ghost::CommandLineInterpreter> getInterpreter() const override;
	std::shared_ptr<ghost::UserManager> getUserManager() const override;
	const ghost::CommandLine& getProgramOptions() const override;
	const std::string& getModuleName() const override;
	void printGhostASCII(const std::string& suffix = "") const override;

protected:
	std::shared_ptr<ghost::ModuleExtension> getExtension(const std::string& typeName) const override;

private:
	bool init();
	bool run();
	void dispose();

	mutable std::mutex _moduleMutex;
	std::string _name;
	ghost::CommandLine _options;
	State _state;
	std::shared_ptr<ThreadPool> _threadPool;
	std::shared_ptr<Console> _console;
	std::shared_ptr<ghost::Logger> _logger;
	std::shared_ptr<UserManager> _userManager;
	std::shared_ptr<CommandLineInterpreter> _interpreter;
	std::vector<std::shared_ptr<ghost::ModuleExtension>> _components;

	std::function<bool(const ghost::Module&)> _initializationBehavior;
	std::function<bool(const ghost::Module&)> _runningBehavior;
	std::function<void(const ghost::Module&)> _disposeBehavior;

	std::mutex _commandExecutorMutex;
	std::condition_variable _commandExecutorCV;
	void commandExecutor();
};
} // namespace internal
} // namespace ghost

#endif // GHOST_MODULE_HPP
