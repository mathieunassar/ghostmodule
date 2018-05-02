#ifndef GHOST_ENTERPRESSEDINPUTEVENT_HPP
#define GHOST_ENTERPRESSEDINPUTEVENT_HPP

#include "InputEvent.hpp"

namespace Ghost
{
	/**
	 *	The user pressed on enter.
	 *	The handler switches the console mode to INPUT if the current mode is OUTPUT.
	 */
	class EnterPressedInputEvent : public InputEvent
	{
	public:
		/* Handler */
		class EnterPressedInputEventHandler : public InputEvent::InputEventHandler
		{
		public:
			EnterPressedInputEventHandler(InputController* controller);
			bool handle(const InputEvent& event) override;

		private:
			const EnterPressedInputEvent& getEvent(const InputEvent& event) const;
		};

		/* Event */
		std::string getEventName() const override;
		static const std::string _NAME;
	};
}

#endif //GHOST_ENTERPRESSEDINPUTEVENT_HPP
