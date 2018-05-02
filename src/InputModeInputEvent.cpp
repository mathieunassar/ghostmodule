#include "../include/InputModeInputEvent.hpp"
#include "../include/InputController.hpp"

using namespace Ghost;

InputModeInputEvent::InputModeInputEventHandler::InputModeInputEventHandler(InputController* controller)
	: InputEventHandler(controller)
{

}

bool InputModeInputEvent::InputModeInputEventHandler::handle(const InputEvent& event)
{
	const InputModeInputEvent& enterEvent = getEvent(event);
	InputController& controller = getController();

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
