#include "../include/Console.hpp"
#include "../include/internal/ConsoleDeviceWindows.hpp"
#include "../include/internal/InputController.hpp"
#include "../include/internal/OutputController.hpp"

#include <iostream>
#include <functional>

using namespace Ghost;

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
	printf("input finished");
	_outputController->stop();

	printf("output finished");
}

void Console::setPrompt(const std::string& prompt)
{
	_inputController->setPrompt(prompt);
}

void Console::setInputMode(InputController::InputMode mode)
{
	_inputController->setInputMode(mode);
}

void Console::write(const std::string& line)
{
	_outputController->write(line);
}

std::string Console::getLine()
{
	_outputController->flush();
	return _inputController->getLine();
}

void Console::onNewInput(const std::string& str)
{
	printf("on new input: %s\n", str.c_str());
}

void Console::onNewMode(internal::ConsoleDevice::ConsoleMode mode)
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
