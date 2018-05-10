#ifndef GHOST_INPUTCONTROLLER_HPP
#define GHOST_INPUTCONTROLLER_HPP

#include <string>

namespace Ghost
{
	/**
	 *	Interface defining methods to manage user input.
	 */
	class InputController
	{
	public:
		enum InputMode
		{
			SEQUENTIAL, // prompt comes back directly after input
			DISCRETE, // prompt only comes back if requested
			NEVER // never prompts except on read
		};

		/// starts the input control
		virtual void start() = 0;
		/// stops the input control
		virtual void stop() = 0;
		/// changes the text displayed by the prompt
		virtual void setPrompt(const std::string& prompt) = 0;
		/// selects the behavior of the console among the possible modes
		virtual void setInputMode(InputMode mode) = 0;
		/// sets the callback that will be called when the user enters a new command
		virtual void setCommandCallback(std::function<void(const std::string&)> cmdCallback) = 0;
		/// gets a line from the user.
		virtual std::string getLine() = 0;
	};
}

#endif // GHOST_INPUTCONTROLLER_HPP
