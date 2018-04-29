#include "../include/InputController.hpp"

#include <iostream>
#include <future>
#include <chrono>

using namespace Ghost;

InputController::InputController(std::shared_ptr<LocalConsole> device,
	LocalConsole::ConsoleMode initialMode,
	std::function<void(const std::string&)> cmdCallback,
	std::function<void(LocalConsole::ConsoleMode)> modeCallback)
	: _threadEnable(false)
	, _device(device)
	, _prompt("> ")
	, _consoleMode(initialMode)
	, _inputMode(DISCRETE)
	, _commandCallback(cmdCallback)
	, _modeCallback(modeCallback)
{
	_device->setConsoleMode(initialMode);
}

void InputController::setPrompt(const std::string& prompt)
{
	_prompt = prompt;
}

void InputController::setInputMode(InputMode mode)
{
	_inputMode = mode;
}

void InputController::start()
{
	if (!_threadEnable)
	{
		_threadEnable = true;
		_inputThread = std::thread(&InputController::inputListenerThread, this);
	}
}

void InputController::stop()
{
	_threadEnable = false;
	if (_inputThread.joinable())
		_inputThread.join();
}

std::string InputController::getLine()
{
	return ""; 
}

void InputController::printPrompt() const
{
	printf(_prompt.c_str());
}

void InputController::switchConsoleMode(LocalConsole::ConsoleMode newMode)
{
	_device->setConsoleMode(newMode);
	_consoleMode = newMode;
	_modeCallback(newMode);
}

void InputController::inputListenerThread()
{
	std::string str = "";
	while (_threadEnable) {
		str = readLine();
		if (str.empty() || _consoleMode == LocalConsole::OUTPUT) // toggle mode
		{
			if (_consoleMode == LocalConsole::OUTPUT)
			{
				switchConsoleMode(LocalConsole::INPUT);
				printPrompt();
			}
			else
			{
				switchConsoleMode(LocalConsole::OUTPUT);
			}
		}
		else if (!str.empty())
		{
			_commandCallback(str);

			// what happens next? if sequential, prompt displays, otherwise the mode is switched back to output
			if (_inputMode == SEQUENTIAL)
				printPrompt();
			else
				switchConsoleMode(LocalConsole::OUTPUT);
		}
	}
}

std::string InputController::readLine()
{
	std::promise<std::string> p;
	auto future = p.get_future();
	
	std::thread t([&p] {
		std::string str;
		std::getline(std::cin, str);
		p.set_value(str);
	});
	
	auto status = future.wait_for(std::chrono::seconds(0));
	while (status != std::future_status::ready && _threadEnable)
	{
		status = future.wait_for(std::chrono::seconds(1));
	}

	if (status == std::future_status::ready)
	{
		t.join();
		return future.get();
	}
	else // the program must exit, t is detached and will die peacefully without triggering exceptions on this one (->legal??)
	{
		t.detach();
		return "";
	}
}
