#ifndef GHOST_INTERNAL_INPUTEVENT_HPP
#define GHOST_INTERNAL_INPUTEVENT_HPP

#include <string>


namespace ghost
{
	namespace internal
	{
		class InputControllerAccess;

		class InputEvent
		{
		public:
			class InputEventHandler
			{
			public:
				InputEventHandler(InputControllerAccess* controller) : _controller(controller) {}
				virtual bool handle(const InputEvent& event) = 0;

			protected:
				InputControllerAccess & getController() const { return *_controller; }

			private:
				InputControllerAccess * _controller;
			};

			virtual std::string getEventName() const = 0;
		};
	}
}

#endif //GHOST_INTERNAL_INPUTEVENT_HPP
