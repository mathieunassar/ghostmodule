#ifndef GHOST_INTERNAL_INPUTCONTROLLER_HPP
#define GHOST_INTERNAL_INPUTCONTROLLER_HPP

#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <map>
#include <functional>

#include "InputControllerAccess.hpp"

#include "ConsoleDevice.hpp"
#include <BlockingQueue.hpp>
#include "InputEvent.hpp"
#include "Prompt.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		 *	Implementation of the InputController.
		 *	Realizes the interface InputControllerAccess which is used by InputEvent objects to access the controller's functionality.
		 */
		class InputController : public InputControllerAccess
		{
		public:
			InputController(std::shared_ptr<ConsoleDevice> device,
				ConsoleDevice::ConsoleMode initialMode,
				std::function<void(const std::string&)> cmdCallback,
				std::function<void(ConsoleDevice::ConsoleMode)> modeCallback);

			void start();
			void stop();

			/// changes the text displayed by the prompt
			Prompt& getPrompt();
			/// selects the behavior of the console among the possible modes
			void setInputMode(InputMode mode);
			/// sets the callback that will be called when the user enters a new command
			void setCommandCallback(std::function<void(const std::string&)> cmdCallback);

			std::string getLine();

			void printPrompt() const override;
			void switchConsoleMode(ConsoleDevice::ConsoleMode newMode) override;
			std::string readLine() override;
			InputMode getInputMode() const override;
			ConsoleDevice::ConsoleMode getConsoleMode() const override;
			void onNewInput(const std::string& input) override;
			std::shared_ptr<std::promise<bool>> onNewEvent(std::shared_ptr<InputEvent> event) override;
			void setLineRequestResult(const std::string& line) override;

		private:
			void registerEventHandlers();

			void inputListenerThread();
			void enterPressedThread();

			/* thread stuff */
			std::thread _inputThread;
			std::atomic<bool> _inputThreadEnable;
			std::thread _enterPressedThread;
			std::atomic<bool> _enterPressedThreadEnable;

			/* Runtime variables */
			BlockingQueue<QueueElement<std::shared_ptr<InputEvent>>> _eventQueue;
			std::shared_ptr<std::string> _explicitInput;

			/* configuration */
			std::shared_ptr<ConsoleDevice> _device;
			std::unique_ptr<Prompt> _prompt;
			ConsoleDevice::ConsoleMode _consoleMode;
			InputMode _inputMode;
			std::function<void(const std::string&)> _commandCallback;
			std::function<void(ConsoleDevice::ConsoleMode)> _modeCallback;
			std::map<std::string, std::shared_ptr<InputEvent::InputEventHandler>> _eventHandlers;
		};
	}
}

#endif // GHOST_INTERNAL_INPUTCONTROLLER_HPP
