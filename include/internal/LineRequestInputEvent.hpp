#ifndef GHOST_INTERNAL_LINEREQUESTINPUTEVENT_HPP
#define GHOST_INTERNAL_LINEREQUESTINPUTEVENT_HPP

#include "InputEvent.hpp"

namespace Ghost
{
	namespace internal
	{
		/**
		*	The user pressed on enter.
		*	The handler switches the console mode to INPUT if the current mode is OUTPUT.
		*/
		class LineRequestInputEvent : public InputEvent
		{
		public:
			/* Handler */
			class LineRequestInputEventHandler : public InputEvent::InputEventHandler
			{
			public:
				LineRequestInputEventHandler(InputControllerAccess* controller);
				bool handle(const InputEvent& event) override;

			private:
				const LineRequestInputEvent& getEvent(const InputEvent& event) const;
			};

			/* Event */
			std::string getEventName() const override;
			static const std::string _NAME;
		};
	}
}

#endif //GHOST_INTERNAL_LINEREQUESTINPUTEVENT_HPP
