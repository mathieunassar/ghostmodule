#include "../include/Console.hpp"
#include "../include/ConsoleDeviceWindows.hpp"

#include <iostream>
#include <functional>

using namespace Ghost;

Console::Console()
	: _device(new ConsoleDeviceWindows())
{
	std::function<void(const std::string&)> cmdCallback = std::bind(&Console::onNewInput, this, std::placeholders::_1);
	std::function<void(ConsoleDevice::ConsoleMode)> modeCallback = std::bind(&Console::onNewMode, this, std::placeholders::_1);
	_inputController = std::make_shared<InputController>(_device, ConsoleDevice::OUTPUT, cmdCallback, modeCallback);
}

void Console::start()
{
	_inputController->start();
}

void Console::stop()
{
	_inputController->stop();
}

void Console::setPrompt(const std::string& prompt)
{
	_inputController->setPrompt(prompt);
}

void Console::setInputMode(InputController::InputMode mode)
{
	_inputController->setInputMode(mode);
}

std::string Console::getLine()
{
	return _inputController->getLine();
}

void Console::onNewInput(const std::string& str)
{
	printf("on new input: %s\n", str.c_str());
}

void Console::onNewMode(ConsoleDevice::ConsoleMode mode)
{
	//printf("new mode received");
}
