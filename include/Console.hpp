#ifndef GHOST_CONSOLE_HPP
#define GHOST_CONSOLE_HPP

#include <string>
#include <functional>

#include "internal/ConsoleDevice.hpp"
#include "InputController.hpp"
namespace Ghost
{
	/**
	 *	Makes abstraction of the program console and allows user i/o with ease.
	 *	Provides methods to write on the console and to read lines from it.
	 *	Per default, the console starts in output mode. In this mode, every call to
	 *	std::cout or to the method "write" of this class will be output on the console
	 *	window.
	 *	The mode switches to the input mode if the user presses on "enter". This mode lets
	 *	the user enter command, which then can be retrieved by setting a callback with the
	 *	method "setCommandCallback".
	 *	Depending on the input mode which can be set by "setInputMode", the mode will be switched
	 *	back to output every time the user enters a command (in case of a "DISCRETE" input mode),
	 *	or solely when the user enters an empty line.
	 *	It is also possible for the program to request a line from the user, with the method
	 *	"getLine". The method will flush the output (write everything on the buffer) and query
	 *	the user.
	 */
	class Console
	{
	public:
		static std::shared_ptr<Console> create();

		/// starts the console. Depending on the implementation, this operation can be unnecessary.
		virtual void start() = 0;
		/// stops the console.
		virtual void stop() = 0;

		/// Sets the text that will be displayed as a prompt in the input mode
		virtual void setPrompt(const std::string& prompt) = 0;
		/// sets the input mode, see {@ref InputController::InputMode} for a description of the modes
		virtual void setInputMode(InputController::InputMode mode) = 0;
		/// sets the callback that will be called when the user enters a new command
		virtual void setCommandCallback(std::function<void(const std::string&)> cmdCallback) = 0;

		/// Enqueues a line to be written on the console. It will be written by a writing thread when
		/// the console is in output mode
		virtual void write(const std::string& line) = 0;
		/// Queries a line from the user.
		virtual std::string getLine() = 0;
		/// flushes the write queue by printing everything
		virtual void flush() = 0;
		/// if the command callback is not set with "setCommandCallback", user commands are collected by the console
		virtual bool hasCommands() const = 0;
		/// if the command callback is not set with "setCommandCallback", user commands are collected by the console
		/// getting a command removes it from the command queue.
		virtual std::string getCommand() = 0;
	};
}

#endif // GHOST_CONSOLE_HPP
