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

#include "Console.hpp"
#ifdef _WIN32
#include "ConsoleDeviceWindows.hpp"
#else
#include "ConsoleDeviceUnix.hpp"
#endif
#include <exception>
#include <functional>
#include <iostream>

#include "InputController.hpp"
#include "OutputController.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::Console> ghost::Console::create(bool redirectStdout)
{
	return std::shared_ptr<ghost::Console>(new ghost::internal::Console(nullptr, redirectStdout));
}

Console::Console(std::shared_ptr<ThreadPool> threadPool, bool redirectStdout)
    : Console(makeConsoleDevice(), threadPool, redirectStdout)
{
}

Console::Console(const std::shared_ptr<ConsoleDevice>& device, std::shared_ptr<ThreadPool> threadPool,
		 bool redirectStdout)
    : _device(device)
{
	if (!threadPool)
	{
		threadPool = std::make_shared<ThreadPool>(std::thread::hardware_concurrency());
		threadPool->start();
	}

	std::function<void(const std::string&)> cmdCallback =
	    std::bind(&Console::onNewInput, this, std::placeholders::_1);
	std::function<void(internal::ConsoleDevice::ConsoleMode)> modeCallback =
	    std::bind(&Console::onNewMode, this, std::placeholders::_1);
	_inputController = std::make_shared<internal::InputController>(
	    threadPool, _device, internal::ConsoleDevice::OUTPUT, cmdCallback, modeCallback);

	_outputController = std::make_shared<internal::OutputController>(_device, redirectStdout);
}

std::shared_ptr<ConsoleDevice> Console::makeConsoleDevice()
{
#ifdef _WIN32
	return std::shared_ptr<internal::ConsoleDevice>(new internal::ConsoleDeviceWindows());
#else
	return std::shared_ptr<internal::ConsoleDevice>(new internal::ConsoleDeviceUnix());
#endif
}

void Console::start()
{
	_inputController->start();
	_outputController->start();
}

void Console::stop()
{
	_inputController->stop(); // stop receiving input first
	_outputController->flush();
	_outputController->stop();
}

void Console::setPromptFormat(const std::string& prompt)
{
	_inputController->getPrompt().setFormat(prompt);
}

void Console::setInputMode(ghost::InputMode mode)
{
	_inputController->setInputMode(mode);
}

void Console::setCommandCallback(std::function<void(const std::string&)> cmdCallback)
{
	_inputController->setCommandCallback(cmdCallback);
}

void Console::write(const std::string& line)
{
	_outputController->write(line);
}

std::string Console::getLine(bool secret)
{
	flush();
	return _inputController->getLine(secret);
}

void Console::flush()
{
	_outputController->flush();
}

bool Console::hasCommands() const
{
	std::unique_lock<std::mutex> lock(_commandQueueLock);
	return _commands.size() != 0;
}

std::string Console::getCommand()
{
	std::unique_lock<std::mutex> lock(_commandQueueLock);

	if (_commands.size() == 0) throw std::logic_error("No command available");

	std::string res = std::move(_commands.front());
	_commands.pop();

	return res;
}

Prompt& Console::getPrompt()
{
	return _inputController->getPrompt();
}

void Console::onNewInput(const std::string& str)
{
	std::unique_lock<std::mutex> lock(_commandQueueLock);
	_commands.push(str);
}

void Console::onNewMode(ConsoleDevice::ConsoleMode mode)
{
	if (mode == internal::ConsoleDevice::OUTPUT)
	{
		_outputController->enable();
	}
	else
	{
		_outputController->disable();
	}
}
