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

#include "EnterPressedInputEvent.hpp"
#include "InputController.hpp"
#include "InputModeInputEvent.hpp"

using namespace ghost::internal;

EnterPressedInputEvent::EnterPressedInputEventHandler::EnterPressedInputEventHandler(InputControllerAccess* controller)
	: InputEventHandler(controller)
{

}

bool EnterPressedInputEvent::EnterPressedInputEventHandler::handle(const InputEvent& event)
{
	InputControllerAccess& controller = getController();

	controller.switchConsoleMode(ConsoleDevice::INPUT);
	controller.printPrompt();
	controller.onNewEvent(std::make_shared<InputModeInputEvent>());
	return true;
}

const EnterPressedInputEvent& EnterPressedInputEvent::EnterPressedInputEventHandler::getEvent(const InputEvent& event) const
{
	return static_cast<const EnterPressedInputEvent&>(event);
}

std::string EnterPressedInputEvent::getEventName() const
{
	return _NAME;
}

const std::string EnterPressedInputEvent::_NAME = "EnterPressed";
