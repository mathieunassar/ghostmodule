#ifndef GHOST_CONSOLE_HPP
#define GHOST_CONSOLE_HPP

#include <string>
#include <memory>

#include "internal/ConsoleDevice.hpp"
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

		std::string getLine();

	private:
		/* Callbacks for the input controller */
		void onNewInput(const std::string& str);
		void onNewMode(internal::ConsoleDevice::ConsoleMode mode);

		std::shared_ptr<internal::ConsoleDevice> _device;
		std::shared_ptr<InputController> _inputController;
	};
}

#endif // GHOST_CONSOLE_HPP
