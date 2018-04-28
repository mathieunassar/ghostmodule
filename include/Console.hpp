#ifndef GHOST_CONSOLE_HPP
#define GHOST_CONSOLE_HPP

#include <string>
#include <thread>
#include <memory>
#include "LocalConsole.hpp"

namespace Ghost
{
	class Console
	{
	public:
		Console();

	private:
		void printPrompt() const;
		void inputListener();
		void onNewInput(const std::string& str);

		std::thread _inputThread;

		std::unique_ptr<LocalConsole> _device;
		LocalConsole::ConsoleMode _mode;
	};
}

#endif // GHOST_CONSOLE_HPP
