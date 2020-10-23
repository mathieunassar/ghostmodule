/*
 * Copyright 2020 Mathieu Nassar
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

#ifndef GHOST_INTERNAL_INPUTCONTROLLERACCESS_HPP
#define GHOST_INTERNAL_INPUTCONTROLLERACCESS_HPP

#include <future>
#include <ghost/module/Console.hpp>
#include <string>

#include "ConsoleDevice.hpp"
#include "InputEvent.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Defines a set of methods used by {@link InputEvent} objects to operate on the InputController.
 */
class InputControllerAccess
{
public:
	virtual ~InputControllerAccess() = default;

	/// Prints the user configured prompt to the console
	virtual void printPrompt() const = 0;
	/// Switches the console mode and calls performs the necessary operations
	virtual void switchConsoleMode(ConsoleDevice::ConsoleMode newMode) = 0;
	/// Reads a line from stdin
	virtual std::string readLine(bool secret) = 0;
	/// Returns the input mode of the input controller, a.k.a. whether commands are entered sequentially or not
	virtual ghost::InputMode getInputMode() const = 0;
	/// Returns the console mode, a.k.a. input or output
	virtual ConsoleDevice::ConsoleMode getConsoleMode() const = 0;
	/// Notifies the input controller that the user entered a new command line
	virtual void onNewInput(const std::string& input) = 0;
	/// Notifies the input controller that a new input event occurred, for example pressed entered while in output
	/// mode
	virtual std::shared_ptr<std::promise<bool>> onNewEvent(std::shared_ptr<InputEvent> event) = 0;
	/// For direct line requests (application requires direct user input); sets the result
	virtual void setLineRequestResult(const std::string& line) = 0;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_INPUTCONTROLLERACCESS_HPP
