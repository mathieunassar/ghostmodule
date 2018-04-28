#include "../include/Console.hpp"
#include "../include/LocalConsoleWindows.hpp"

#include <iostream>

using namespace Ghost;

Console::Console()
	: _device(new LocalConsoleWindows())
	, _mode(LocalConsole::OUTPUT)
{
	_device->setConsoleMode(LocalConsole::OUTPUT);
	_inputThread = std::thread(&Console::inputListener, this);
}

void Console::printPrompt() const
{
	printf("> ");
}

void Console::inputListener()
{
	std::string s = "";
	while (std::getline(std::cin, s)) {
		onNewInput(s);
	}
}

void Console::onNewInput(const std::string& str)
{
	if (str.empty() || _mode == LocalConsole::OUTPUT) // toggle mode
	{
		if (_mode == LocalConsole::OUTPUT)
		{
			_device->setConsoleMode(LocalConsole::INPUT);
			_mode = LocalConsole::INPUT;
			printf("Switched to INPUT mode\n");
			printPrompt();
		}
		else
		{
			_device->setConsoleMode(LocalConsole::OUTPUT);
			_mode = LocalConsole::OUTPUT;
			printf("Switched to OUTPUT mode\n");
		}
	}
	else if (_mode == LocalConsole::INPUT)
	{
		printPrompt();
	}
}
