#ifndef GHOST_INPUTCONTROLLER_HPP
#define GHOST_INPUTCONTROLLER_HPP

#include <string>
#include <thread>
#include <atomic>
#include <memory>

#include "LocalConsole.hpp"

namespace Ghost
{
	class InputController
	{
	public:
		enum InputMode
		{
			SEQUENTIAL, // prompt comes back directly after input
			DISCRETE, // prompt only comes back if requested
			NEVER // never prompts except on read
		};

		InputController(std::shared_ptr<LocalConsole> device,
			LocalConsole::ConsoleMode initialMode,
			std::function<void(const std::string&)> cmdCallback,
			std::function<void(LocalConsole::ConsoleMode)> modeCallback);

		void start();
		void stop();

		/// changes the text displayed by the prompt
		void setPrompt(const std::string& prompt);
		/// selects the behavior of the console among the possible modes
		void setInputMode(InputMode mode);

		// implement here the "explicit read"
		std::string getLine();

	private:
		void printPrompt() const;
		void switchConsoleMode(LocalConsole::ConsoleMode newMode);
		std::string readLine();
		void inputListenerThread();

		std::thread _inputThread;
		std::atomic<bool> _threadEnable;

		std::shared_ptr<LocalConsole> _device;
		std::string _prompt;
		LocalConsole::ConsoleMode _consoleMode;
		InputMode _inputMode;
		std::function<void(const std::string&)> _commandCallback;
		std::function<void(LocalConsole::ConsoleMode)> _modeCallback;
	};
}

#endif // GHOST_INPUTCONTROLLER_HPP
