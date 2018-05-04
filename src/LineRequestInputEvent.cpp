#include "../include/internal/LineRequestInputEvent.hpp"
#include "../include/internal/InputController.hpp"

using namespace Ghost::internal;

LineRequestInputEvent::LineRequestInputEventHandler::LineRequestInputEventHandler(InputControllerAccess* controller)
	: InputEventHandler(controller)
{

}

bool LineRequestInputEvent::LineRequestInputEventHandler::handle(const InputEvent& event)
{
	const LineRequestInputEvent& enterEvent = getEvent(event);
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
