#include "../include/internal/InputModeInputEvent.hpp"
#include "../include/internal/InputController.hpp"

using namespace Ghost::internal;

InputModeInputEvent::InputModeInputEventHandler::InputModeInputEventHandler(InputControllerAccess* controller)
	: InputEventHandler(controller)
{

}

bool InputModeInputEvent::InputModeInputEventHandler::handle(const InputEvent& event)
{
	const InputModeInputEvent& enterEvent = getEvent(event);
	InputControllerAccess& controller = getController();

	std::string line;
	bool loop = true;

	while (loop)
	{
		line = controller.readLine();
		
		if (line.empty())
		{
			controller.switchConsoleMode(ConsoleDevice::OUTPUT);
			loop = false;
		}
		else
		{
			controller.onNewInput(line);

			// what happens next? if sequential, prompt displays, otherwise the mode is switched back to output
			if (controller.getInputMode() == InputController::SEQUENTIAL)
			{
				controller.printPrompt();
				// loop is true
			}
			else
			{
				controller.switchConsoleMode(ConsoleDevice::OUTPUT);
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
