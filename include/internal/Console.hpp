#ifndef GHOST_INTERNAL_CONSOLE_HPP
#define GHOST_INTERNAL_CONSOLE_HPP

#include <string>
#include <memory>
#include <queue>
#include <mutex>

#include "../Console.hpp"
#include "ConsoleDevice.hpp"
#include "InputController.hpp"
#include "OutputController.hpp"

namespace ghost
{
	namespace internal
	{
		class Console : public ghost::Console
		{
		public:
			Console();

			void start() override;
			void stop() override;

			void setPromptFormat(const std::string& prompt) override;
			void setInputMode(InputController::InputMode mode) override;
			void setCommandCallback(std::function<void(const std::string&)> cmdCallback) override;

			void write(const std::string& line) override;
			std::string getLine() override;
			void flush() override;
			bool hasCommands() const override;
			std::string getCommand() override;

			Prompt& getPrompt();

		private:
			/* Callbacks for the input controller */
			void onNewInput(const std::string& str);
			void onNewMode(ConsoleDevice::ConsoleMode mode);

			mutable std::mutex _commandQueueLock;
			std::queue<std::string> _commands;

			std::shared_ptr<internal::ConsoleDevice> _device;
			std::shared_ptr<InputController> _inputController;
			std::shared_ptr<OutputController> _outputController;
		};
	}
}

#endif // GHOST_INTERNAL_CONSOLE_HPP
