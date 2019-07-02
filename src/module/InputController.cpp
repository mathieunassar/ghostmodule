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

#include "InputController.hpp"

#include <iostream>
#include <future>
#include <chrono>

#include "EnterPressedInputEvent.hpp"
#include "LineRequestInputEvent.hpp"
#include "InputModeInputEvent.hpp"

using namespace ghost::internal;

InputController::InputController(std::shared_ptr<ConsoleDevice> device,
	ConsoleDevice::ConsoleMode initialMode,
	std::function<void(const std::string&)> cmdCallback,
	std::function<void(ConsoleDevice::ConsoleMode)> modeCallback)
	: _inputThreadEnable(false)
	, _enterPressedThreadEnable(false)
	, _device(device)
	, _prompt(new Prompt(">"))
	, _consoleMode(initialMode)
	, _inputMode(ghost::InputMode::DISCRETE)
	, _commandCallback(cmdCallback)
	, _modeCallback(modeCallback)
{
	registerEventHandlers();

	_device->setConsoleMode(initialMode);
}

Prompt& InputController::getPrompt()
{
	return *_prompt;
}

void InputController::setInputMode(ghost::InputMode mode)
{
	_inputMode = mode;
}

void InputController::setCommandCallback(std::function<void(const std::string&)> cmdCallback)
{
	_commandCallback = cmdCallback;
}

void InputController::start()
{
	if (!_enterPressedThreadEnable)
	{
		_device->start();

		_enterPressedThreadEnable = true;
		_inputThreadEnable = true;
		_inputThread = std::thread(&InputController::inputListenerThread, this);
		_enterPressedThread = std::thread(&InputController::enterPressedThread, this);
	}
}

void InputController::stop()
{
	_device->stop();

	// join this thread first to avoid that it waits for input thread which would be already finished
	if (_enterPressedThreadEnable)
	{
		_enterPressedThreadEnable = false;
		if (_enterPressedThread.joinable())
			_enterPressedThread.join();
	}

	if (_inputThreadEnable)
	{
		_inputThreadEnable = false;
		if (_inputThread.joinable())
			_inputThread.join();
	}
}

std::string InputController::getLine()
{
	_explicitInput = nullptr;

	auto promise = onNewEvent(std::make_shared<LineRequestInputEvent>()); // here wait that the event is completed
	promise->get_future().wait();

	if (_explicitInput)
	{
		return *_explicitInput;
	}
	return "";
}

void InputController::printPrompt() const
{
	_device->write(_prompt->str());
}

void InputController::switchConsoleMode(ConsoleDevice::ConsoleMode newMode)
{
	_device->setConsoleMode(newMode);
	_consoleMode = newMode;
	_modeCallback(newMode);
}

void InputController::registerEventHandlers()
{
	_eventHandlers[EnterPressedInputEvent::_NAME] = std::make_shared<EnterPressedInputEvent::EnterPressedInputEventHandler>(this);
	_eventHandlers[LineRequestInputEvent::_NAME] = std::make_shared<LineRequestInputEvent::LineRequestInputEventHandler>(this);
	_eventHandlers[InputModeInputEvent::_NAME] = std::make_shared<InputModeInputEvent::InputModeInputEventHandler>(this);
}

void InputController::inputListenerThread()
{
	while (_inputThreadEnable)
	{
		QueueElement<std::shared_ptr<InputEvent>> event;
		if (!_eventQueue.tryGet(std::chrono::milliseconds(100), event)) // tryget only blocks for a second
		{
			continue;
		}
		
		_eventQueue.pop();
		auto& handler = _eventHandlers.at(event.element->getEventName());
		bool success = handler->handle(*event.element);
		event.result->set_value(success);

		if (!success)
		{
			std::cout << "Failed to handle event of type: " << event.element->getEventName() << std::endl;
		}
	}
}

void InputController::enterPressedThread()
{
	while (_enterPressedThreadEnable)
	{
		if (_consoleMode == ConsoleDevice::INPUT)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		if (_device->awaitInputMode() && _consoleMode == ConsoleDevice::OUTPUT)
		{
			auto promise = onNewEvent(std::make_shared<EnterPressedInputEvent>()); // here wait that the event is completed
			promise->get_future().wait();
		}
	}
}

std::string InputController::readLine()
{
	std::string readLine;
	bool readSuccess = _device->read(readLine);
	if (!readSuccess)
		return "";
	
	return readLine;
}

ghost::InputMode InputController::getInputMode() const
{
	return _inputMode;
}

void InputController::onNewInput(const std::string& input)
{
	_commandCallback(input);
}

std::shared_ptr<std::promise<bool>> InputController::onNewEvent(std::shared_ptr<InputEvent> event)
{
	QueueElement<std::shared_ptr<InputEvent>> element;
	element.element = event;
	element.result = std::make_shared<std::promise<bool>>();
	_eventQueue.push(element);
	return std::move(element.result);
}

void InputController::setLineRequestResult(const std::string& line)
{
	_explicitInput = std::make_shared<std::string>(line);
}

ConsoleDevice::ConsoleMode InputController::getConsoleMode() const
{
	return _consoleMode;
}
