/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GHOST_INTERNAL_CONSOLE_HPP
#define GHOST_INTERNAL_CONSOLE_HPP

#include <string>
#include <memory>
#include <queue>
#include <mutex>

#include <ghost/module/Console.hpp>
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
			Console(bool redirectStdout = true);
			Console(const std::shared_ptr<ConsoleDevice>& device, bool redirectStdout = true);

			void start() override;
			void stop() override;

			void setPromptFormat(const std::string& prompt) override;
			void setInputMode(InputMode mode) override;
			void setCommandCallback(std::function<void(const std::string&)> cmdCallback) override;

			void write(const std::string& line) override;
			std::string getLine() override;
			void flush() override;
			bool hasCommands() const override;
			std::string getCommand() override;

			Prompt& getPrompt();

			void onNewInput(const std::string& str);

		private:
			/* Callbacks for the input controller */
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
