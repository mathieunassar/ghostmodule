/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GHOST_CONSOLE_HPP
#define GHOST_CONSOLE_HPP

#include <string>
#include <functional>

namespace ghost
{
	enum class InputMode
	{
		SEQUENTIAL, // prompt comes back directly after input
		DISCRETE, // prompt only comes back if requested
		NEVER // never prompts except on read
	};

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
		virtual ~Console() = 0;

		/// starts the console. Depending on the implementation, this operation can be unnecessary.
		virtual void start() = 0;
		/// stops the console.
		virtual void stop() = 0;

		/// Sets the text that will be displayed as a prompt in the input mode
		virtual void setPromptFormat(const std::string& prompt) = 0;
		/// sets the input mode, see {@ref InputController::InputMode} for a description of the modes
		virtual void setInputMode(ghost::InputMode mode) = 0;
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

	inline Console::~Console() {}
}

#endif // GHOST_CONSOLE_HPP
