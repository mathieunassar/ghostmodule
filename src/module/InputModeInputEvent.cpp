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

#include "InputModeInputEvent.hpp"

#include "InputController.hpp"

using namespace ghost::internal;

InputModeInputEvent::InputModeInputEventHandler::InputModeInputEventHandler(InputControllerAccess* controller)
    : InputEventHandler(controller)
{
}

bool InputModeInputEvent::InputModeInputEventHandler::handle(const InputEvent& event)
{
	InputControllerAccess& controller = getController();

	std::string line;
	bool loop = true;

	while (loop)
	{
		line = controller.readLine(false);

		if (line.empty())
		{
			controller.switchConsoleMode(ConsoleDevice::OUTPUT);
			loop = false;
		}
		else
		{
			controller.switchConsoleMode(ConsoleDevice::OUTPUT);
			controller.onNewInput(line);

			// what happens next? if sequential, prompt displays, otherwise the mode is switched back to
			// output
			if (controller.getInputMode() == ghost::InputMode::SEQUENTIAL)
			{
				// TODO here wait that the command is processed
				controller.switchConsoleMode(ConsoleDevice::INPUT);
				controller.printPrompt();
				// loop is true
			}
			else
			{
				loop = false;
			}
		}
	}

	return true;
}

const InputModeInputEvent& InputModeInputEvent::InputModeInputEventHandler::getEvent(const InputEvent& event) const
{
	return static_cast<const InputModeInputEvent&>(event);
}

std::string InputModeInputEvent::getEventName() const
{
	return _NAME;
}

const std::string InputModeInputEvent::_NAME = "InputMode";
