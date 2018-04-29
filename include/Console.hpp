#ifndef GHOST_CONSOLE_HPP
#define GHOST_CONSOLE_HPP

#include "LocalConsole.hpp"
#include "InputController.hpp"

namespace Ghost
{
	class Console
	{
	public:
		Console();

		void start();
		void stop();

		void setPrompt(const std::string& prompt);
		void setInputMode(InputController::InputMode mode);

	private:
		/* Callbacks for the input controller */
		void onNewInput(const std::string& str);
		void onNewMode(LocalConsole::ConsoleMode mode);

		std::shared_ptr<LocalConsole> _device;
		std::shared_ptr<InputController> _inputController;
	};
}

#endif // GHOST_CONSOLE_HPP
