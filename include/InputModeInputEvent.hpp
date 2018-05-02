#ifndef GHOST_INPUTMODEINPUTEVENT_HPP
#define GHOST_INPUTMODEINPUTEVENT_HPP

#include "InputEvent.hpp"

namespace Ghost
{
	/**
	*	The user pressed on enter.
	*	The handler switches the console mode to INPUT if the current mode is OUTPUT.
	*/
	class InputModeInputEvent : public InputEvent
	{
	public:
		/* Handler */
		class InputModeInputEventHandler : public InputEvent::InputEventHandler
		{
		public:
			InputModeInputEventHandler(InputController* controller);
			bool handle(const InputEvent& event) override;

		private:
			const InputModeInputEvent& getEvent(const InputEvent& event) const;
		};

		/* Event */
		std::string getEventName() const override;
		static const std::string _NAME;
	};
}

#endif //GHOST_INPUTMODEINPUTEVENT_HPP
