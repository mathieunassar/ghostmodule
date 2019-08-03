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

#include "LineRequestInputEvent.hpp"
#include "InputController.hpp"

using namespace ghost::internal;

LineRequestInputEvent::LineRequestInputEventHandler::LineRequestInputEventHandler(InputControllerAccess* controller)
	: InputEventHandler(controller)
{

}

bool LineRequestInputEvent::LineRequestInputEventHandler::handle(const InputEvent& event)
{
	InputControllerAccess& controller = getController();

	ConsoleDevice::ConsoleMode oldState = controller.getConsoleMode();
	controller.switchConsoleMode(ConsoleDevice::INPUT);

	std::string line = controller.readLine();
	controller.setLineRequestResult(line);
	
	controller.switchConsoleMode(oldState);

	return true;
}

const LineRequestInputEvent& LineRequestInputEvent::LineRequestInputEventHandler::getEvent(const InputEvent& event) const
{
	return static_cast<const LineRequestInputEvent&>(event);
}

std::string LineRequestInputEvent::getEventName() const
{
	return _NAME;
}

const std::string LineRequestInputEvent::_NAME = "LineRequest";
