#include "../include/internal/Console.hpp"
#include "../include/internal/ConsoleDeviceWindows.hpp"
#include "../include/internal/InputController.hpp"
#include "../include/internal/OutputController.hpp"

#include <iostream>
#include <functional>
#include <exception>

using namespace Ghost::internal;

std::shared_ptr<Ghost::Console> Ghost::Console::create()
{
	return std::shared_ptr<Ghost::Console>(new Ghost::internal::Console());
}

Console::Console()
	: _device(new internal::ConsoleDeviceWindows())
{
	std::function<void(const std::string&)> cmdCallback = std::bind(&Console::onNewInput, this, std::placeholders::_1);
	std::function<void(internal::ConsoleDevice::ConsoleMode)> modeCallback = std::bind(&Console::onNewMode, this, std::placeholders::_1);
	_inputController = std::make_shared<internal::InputController>(_device, internal::ConsoleDevice::OUTPUT, cmdCallback, modeCallback);
	_outputController = std::make_shared<internal::OutputController>();
}

void Console::start()
{
	_inputController->start();
	_outputController->start();
}

void Console::stop()
{
	_inputController->stop();
	_outputController->stop();
}

void Console::setPrompt(const std::string& prompt)
{
	_inputController->setPrompt(prompt);
}

void Console::setInputMode(InputController::InputMode mode)
{
	_inputController->setInputMode(mode);
}

void Console::setCommandCallback(std::function<void(const std::string&)> cmdCallback)
{
	_inputController->setCommandCallback(cmdCallback);
}

void Console::write(const std::string& line)
{
	_outputController->write(line);
}

std::string Console::getLine()
{
	flush();
	return _inputController->getLine();
}

void Console::flush()
{
	_outputController->flush();
}

bool Console::hasCommands() const
{
	std::unique_lock<std::mutex> lock(_commandQueueLock);
	return _commands.size() != 0;
}

std::string Console::getCommand()
{
	std::unique_lock<std::mutex> lock(_commandQueueLock);

	if (_commands.size() == 0)
		throw std::logic_error("No command available");

	std::string res = std::move(_commands.front());
	_commands.pop();

	return res;
}

void Console::onNewInput(const std::string& str)
{
	std::unique_lock<std::mutex> lock(_commandQueueLock);
	_commands.push(str);
}

void Console::onNewMode(ConsoleDevice::ConsoleMode mode)
{
	
	if (mode == internal::ConsoleDevice::OUTPUT)
	{
		_outputController->enable();
	}
	else
	{
		_outputController->disable();
	}
}
