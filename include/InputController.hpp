#ifndef GHOST_INPUTCONTROLLER_HPP
#define GHOST_INPUTCONTROLLER_HPP

#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <map>

#include "ConsoleDevice.hpp"
#include "BlockingQueue.hpp"
#include "InputEvent.hpp"

namespace Ghost
{
	// split to processor interface and public interface
	class InputController
	{
	public:
		enum InputMode
		{
			SEQUENTIAL, // prompt comes back directly after input
			DISCRETE, // prompt only comes back if requested
			NEVER // never prompts except on read
		};

		InputController(std::shared_ptr<ConsoleDevice> device,
			ConsoleDevice::ConsoleMode initialMode,
			std::function<void(const std::string&)> cmdCallback,
			std::function<void(ConsoleDevice::ConsoleMode)> modeCallback);

		void start();
		void stop();

		/// changes the text displayed by the prompt
		void setPrompt(const std::string& prompt);
		/// selects the behavior of the console among the possible modes
		void setInputMode(InputMode mode);

		std::string getLine();

		void printPrompt() const;
		void switchConsoleMode(ConsoleDevice::ConsoleMode newMode);
		std::string readLine();
		InputMode getInputMode() const;
		ConsoleDevice::ConsoleMode getConsoleMode() const;
		void onNewInput(const std::string& input);
		std::promise<bool>& onNewEvent(std::shared_ptr<InputEvent> event);
		void setLineRequestResult(const std::string& line);

	private:
		void registerEventHandlers();

		void inputListenerThread();
		void enterPressedThread();

		/* thread stuff */
		std::thread _inputThread;
		std::thread _enterPressedThread;
		std::atomic<bool> _threadEnable;

		/* explicit read stuff */
		std::atomic<bool> _explicitRead;
		std::condition_variable _explicitTrigger;
		std::mutex _explicitTriggerLock;
		std::shared_ptr<std::string> _explicitInput;

		BlockingQueue<std::shared_ptr<InputEvent>> _eventQueue;

		/* configuration */
		std::shared_ptr<ConsoleDevice> _device;
		std::string _prompt;
		ConsoleDevice::ConsoleMode _consoleMode;
		InputMode _inputMode;
		std::function<void(const std::string&)> _commandCallback;
		std::function<void(ConsoleDevice::ConsoleMode)> _modeCallback;
		std::map<std::string, std::shared_ptr<InputEvent::InputEventHandler>> _eventHandlers;
	};
}

#endif // GHOST_INPUTCONTROLLER_HPP
