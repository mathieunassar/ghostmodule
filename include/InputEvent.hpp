#ifndef GHOST_INPUTEVENT_HPP
#define GHOST_INPUTEVENT_HPP

#include <string>


namespace Ghost
{
	class InputController;

	class InputEvent
	{
	public:
		class InputEventHandler
		{
		public:
			InputEventHandler(InputController* controller) : _controller(controller) {}
			virtual bool handle(const InputEvent& event) = 0;

		protected:
			InputController& getController() const { return *_controller; }

		private:
			InputController* _controller;
		};

		virtual std::string getEventName() const = 0;
	};
}

#endif //GHOST_INPUTEVENT_HPP
