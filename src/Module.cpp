#include "../include/internal/Module.hpp"
#include "../include/Module.hpp"

#include "../include/internal/commands/ExitCommand.hpp"

using namespace ghost::internal;

Module::Module(const std::string& name)
	: _name(name)
	, _state(Module::STOPPED)
{
	_userManager = std::shared_ptr<UserManager>(new UserManager());
	_interpreter = std::shared_ptr<CommandLineInterpreter>(new CommandLineInterpreter(_userManager));

	// add useful commands
	_interpreter->registerCommand(std::shared_ptr<ghost::Command>(new ExitCommand(this)), {});

}

Module::~Module()
{
	if (hasConsole())
		_console->stop();
}

bool Module::setState(State state)
{
	switch (state)
	{
	case INITIALIZING:
		if (_state != STOPPED) // cannot initialize if alread initialized
			return false;
		break;
	case RUNNING:
		if (_state != INITIALIZING)
			return false;
		break;
	case DISPOSING:
		if (_state == STOPPED) // cannot dispose if already stopped
			return false;
		break;
	default: break;
	}

	_state = state;
	return true;
}

Module::State Module::getState() const
{
	return _state;
}

void Module::initializeConsole()
{
	_console = std::shared_ptr<Console>(new Console());

	_console->setCommandCallback([this](const std::string& str)
		{ 
			_interpreter->execute(str);
			_console->flush();
		});
	
	_userManager->setConnectedUserCallback([this](const ghost::User& user)
		{
			_console->getPrompt().setUser(user.getName());
		});

	_console->start();
}

bool Module::hasConsole() const
{
	return _console.operator bool();
}

std::shared_ptr<ghost::Console> Module::getConsole()
{
	return _console;
}

std::shared_ptr<ghost::CommandLineInterpreter> Module::getInterpreter()
{
	return _interpreter;
}

std::shared_ptr<ghost::UserManager> Module::getUserManager()
{
	return _userManager;
}

void Module::printGhostASCII()
{
	std::cout << " _______ _      _  ______  ________ _________" << std::endl;
	std::cout << "|  _____ |______| |      | |_______     |" << std::endl;
	std::cout << "|______| |      | |______| _______|     |" << std::endl;

	if (hasConsole())
		getConsole()->flush();
}

/////////////////////////////////////////////////////////////////
//////////// DEFINITION OF PUBLIC CLASS MODULE //////////////////

ghost::Module::Module(const std::string& name)
	: _internal(std::shared_ptr<ghost::internal::Module>(new ghost::internal::Module(name)))
{

}

void ghost::Module::start()
{
	if (_internal->getState() != ghost::internal::Module::STOPPED) // only start if module is stopped
		return;

	_internal->setState(ghost::internal::Module::INITIALIZING);
	bool initSuccess = init(); // initialize the module
	if (!initSuccess)
	{
		_internal->setState(ghost::internal::Module::DISPOSING);
		dispose(); // if initialization failed, dispose the module
		_internal->setState(ghost::internal::Module::STOPPED);
	}
	else
	{
		_internal->setState(ghost::internal::Module::RUNNING);
		bool runFinshed = false;
		ghost::internal::Module::State currentState = _internal->getState();
		while (!runFinshed && currentState == ghost::internal::Module::RUNNING)
		{
			runFinshed = !run(); // run as long as the return value is true and the module state is running
			currentState = _internal->getState();
		}

		_internal->setState(ghost::internal::Module::DISPOSING);
		dispose();
		_internal->setState(ghost::internal::Module::STOPPED);
	}
}

void ghost::Module::sleepMillisecond(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

bool ghost::Module::init()
{
	// per default do nothing
	return true;
}

void ghost::Module::dispose()
{
	// per default do nothing
}

void ghost::Module::initializeConsole()
{
	return _internal->initializeConsole();
}

bool ghost::Module::hasConsole() const
{
	return _internal->hasConsole();
}

std::shared_ptr<ghost::Console> ghost::Module::getConsole()
{
	return _internal->getConsole();
}

std::shared_ptr<ghost::CommandLineInterpreter> ghost::Module::getInterpreter()
{
	return _internal->getInterpreter();
}

std::shared_ptr<ghost::UserManager> ghost::Module::getUserManager()
{
	return _internal->getUserManager();
}

void ghost::Module::printGhostASCII()
{
	return _internal->printGhostASCII();
}
