#include "../include/EnterPressedInputEvent.hpp"
#include "../include/InputController.hpp"
#include "../include/InputModeInputEvent.hpp"

using namespace Ghost;

EnterPressedInputEvent::EnterPressedInputEventHandler::EnterPressedInputEventHandler(InputController* controller)
	: InputEventHandler(controller)
{

}

bool EnterPressedInputEvent::EnterPressedInputEventHandler::handle(const InputEvent& event)
{
	const EnterPressedInputEvent& enterEvent = getEvent(event);
	InputController& controller = getController();

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
