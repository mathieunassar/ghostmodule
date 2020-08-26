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

#include "Module.hpp"

#include <ghost/module/Module.hpp>

#include "commands/ExitCommand.hpp"

using namespace ghost::internal;

Module::Module(const std::string& name, const std::shared_ptr<ThreadPool>& threadPool,
	       const std::shared_ptr<Console>& console, const std::shared_ptr<ghost::Logger>& logger,
	       const ghost::CommandLine& options,
	       const std::vector<std::shared_ptr<ghost::ModuleExtension>>& components,
	       const std::function<bool(const ghost::Module&)>& initializationBehavior,
	       const std::function<bool(const ghost::Module&)>& runningBehavior,
	       const std::function<void(const ghost::Module&)>& disposeBehavior)
    : _name(name)
    , _options(options)
    , _state(Module::STOPPED)
    , _threadPool(threadPool)
    , _console(console)
    , _logger(logger)
    , _components(components)
    , _initializationBehavior(initializationBehavior)
    , _runningBehavior(runningBehavior)
    , _disposeBehavior(disposeBehavior)
{
	_userManager = std::shared_ptr<UserManager>(new UserManager());
	_interpreter = std::shared_ptr<CommandLineInterpreter>(new CommandLineInterpreter(_userManager, logger));

	// add useful commands
	_interpreter->registerCommand(std::shared_ptr<ghost::Command>(new ExitCommand(this)), {});

	if (_console)
	{
		_console->setCommandCallback([this](const std::string& str) {
			_console->onNewInput(str);
			_commandExecutorCV.notify_one();
		});

		_userManager->setConnectedUserCallback(
		    [this](std::shared_ptr<ghost::User> user) {
			    if (user)
				    _console->getPrompt().setUser(user->getName());
			    else
				    _console->getPrompt().setUser("");
		    },
		    ghost::Session::createLocal());
	}
}

Module::~Module()
{
	if (_console) _console->stop();

	// The thread pool is the last thing to stop, to give a chance to the components to clean-up their executors
	_threadPool->stop(true);
}

bool Module::setState(State state)
{
	std::unique_lock<std::mutex> lock(_moduleMutex);
	switch (state)
	{
		case INITIALIZING:
			if (_state != STOPPED) // cannot initialize if alread initialized
				return false;
			break;
		case RUNNING:
			if (_state != INITIALIZING) return false;
			break;
		case DISPOSING:
			if (_state == STOPPED) // cannot dispose if already stopped
				return false;
			break;
		default:
			break;
	}

	_state = state;
	return true;
}

Module::State Module::getState() const
{
	std::unique_lock<std::mutex> lock(_moduleMutex);
	return _state;
}

// From ghost::Module
void Module::start()
{
	if (getState() != ghost::internal::Module::STOPPED) // only start if module is stopped
		return;

	// Start the thread pool
	_threadPool->start();

	if (_console) _console->start();

	// Call the initialization routine
	setState(ghost::internal::Module::INITIALIZING);
	bool initSuccess = init(); // initialize the module
	if (!initSuccess)
	{
		stop();
		return;
	}

	// Start the components
	bool componentsStarted = true;
	for (const auto& component : _components) componentsStarted = componentsStarted && component->start();
	if (!componentsStarted)
	{
		stop();
		return;
	}

	// Set the module to running
	setState(ghost::internal::Module::RUNNING);
	if (_console)
		_threadPool->makeScheduledExecutor()->scheduleAtFixedRate(std::bind(&Module::commandExecutor, this),
									  std::chrono::milliseconds(10));

	// Call the running routine as long as the module is running and it returns true
	bool runFinshed = false;
	ghost::internal::Module::State currentState = getState();
	while (!runFinshed && currentState == ghost::internal::Module::RUNNING)
	{
		runFinshed = !run(); // run as long as the return value is true and the module state is running
		currentState = getState();
	}

	// Dipose the module
	if (currentState == ghost::internal::Module::RUNNING || currentState == ghost::internal::Module::DISPOSING)
		stop();
}

void Module::stop()
{
	if (getState() == ghost::internal::Module::STOPPED ||
	    getState() == ghost::internal::Module::DISPOSING) // only stop if module is running or intializing
		return;

	setState(ghost::internal::Module::DISPOSING);
	dispose();
	setState(ghost::internal::Module::STOPPED);

	// Stop the components
	for (const auto& component : _components) component->stop();

	_threadPool->stop(true);
	if (_console) _console->stop();
}

std::shared_ptr<ghost::Console> Module::getConsole() const
{
	return _console;
}

std::shared_ptr<ghost::Logger> Module::getLogger() const
{
	return _logger;
}

std::shared_ptr<ghost::CommandLineInterpreter> Module::getInterpreter() const
{
	return _interpreter;
}

std::shared_ptr<ghost::UserManager> Module::getUserManager() const
{
	return _userManager;
}

const ghost::CommandLine& Module::getProgramOptions() const
{
	return _options;
}

std::shared_ptr<ghost::ThreadPool> Module::getThreadPool() const
{
	return _threadPool;
}

const std::string& Module::getModuleName() const
{
	return _name;
}

void Module::printGhostASCII(const std::string& suffix) const
{
	std::string s = "";
	if (!suffix.empty()) s = "/" + suffix;

	if (_logger)
	{
		GHOST_INFO(getLogger()) << " _______ _      _  ______  ________ _________";
		GHOST_INFO(getLogger()) << "|  _____ |______| |      | |_______     |";
		GHOST_INFO(getLogger()) << "|______| |      | |______| _______|     |" << s;
		GHOST_INFO(getLogger()) << "";
	}
	else
	{
		std::cout << " _______ _      _  ______  ________ _________" << std::endl;
		std::cout << "|  _____ |______| |      | |_______     |" << std::endl;
		std::cout << "|______| |      | |______| _______|     |" << s << std::endl;
		std::cout << std::endl;
	}

	auto console = getConsole();
	if (console) console->flush();
}

std::shared_ptr<ghost::ModuleExtension> Module::getExtension(const std::string& typeName) const
{
	for (const auto& c : _components)
	{
		if (c->getName() == typeName) return c;
	}
	return nullptr;
}

bool Module::init()
{
	if (_initializationBehavior) return _initializationBehavior(*this);

	// per default do nothing
	return true;
}

bool Module::run()
{
	if (_runningBehavior) return _runningBehavior(*this);

	// per default do nothing
	return false;
}

void Module::dispose()
{
	if (_disposeBehavior) return _disposeBehavior(*this);

	// per default do nothing
}

void Module::commandExecutor()
{
	std::unique_lock<std::mutex> lock(_commandExecutorMutex);
	bool waitResult = _commandExecutorCV.wait_for(lock, std::chrono::milliseconds(1), [&] {
		return getState() != ghost::internal::Module::RUNNING || _console->hasCommands();
	});
	if (!waitResult) return;

	if (getState() == ghost::internal::Module::RUNNING && _console->hasCommands())
	{
		auto command = _console->getCommand();
		ghost::CommandExecutionContext context(ghost::Session::createLocal());
		context.setConsole(_console);
		_interpreter->execute(command, context);
		_console->flush();
	}
}
