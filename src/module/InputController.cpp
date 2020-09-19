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

#include <chrono>
#include <future>
#include <iostream>

#include "EnterPressedInputEvent.hpp"
#include "InputModeInputEvent.hpp"
#include "LineRequestInputEvent.hpp"

using namespace ghost::internal;

InputController::InputController(const std::shared_ptr<ThreadPool>& threadPool, std::shared_ptr<ConsoleDevice> device,
				 ConsoleDevice::ConsoleMode initialMode,
				 std::function<void(const std::string&)> cmdCallback,
				 std::function<void(ConsoleDevice::ConsoleMode)> modeCallback)
    : _threadPool(threadPool)
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

InputController::~InputController()
{
	stop();
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
	if (!_executor)
	{
		_device->start();

		_executor = _threadPool->makeScheduledExecutor();
		_executor->scheduleAtFixedRate(std::bind(&InputController::inputListenerThread, this),
					       std::chrono::milliseconds(1));
		_executor->scheduleAtFixedRate(std::bind(&InputController::enterPressedThread, this),
					       std::chrono::milliseconds(1));
	}
}

void InputController::stop()
{
	_device->stop();

	if (_executor)
	{
		_executor->stop();
		_executor.reset();
	}
}

std::string InputController::getLine(bool secret)
{
	_explicitInput = nullptr;

	auto promise =
	    onNewEvent(std::make_shared<LineRequestInputEvent>(secret)); // here wait that the event is completed
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
	_eventHandlers[EnterPressedInputEvent::_NAME] =
	    std::make_shared<EnterPressedInputEvent::EnterPressedInputEventHandler>(this);
	_eventHandlers[LineRequestInputEvent::_NAME] =
	    std::make_shared<LineRequestInputEvent::LineRequestInputEventHandler>(this);
	_eventHandlers[InputModeInputEvent::_NAME] =
	    std::make_shared<InputModeInputEvent::InputModeInputEventHandler>(this);
}

void InputController::inputListenerThread()
{
	QueueElement<std::shared_ptr<InputEvent>> event;
	if (!_eventQueue.tryGet(std::chrono::milliseconds(1), event)) return; // tryget only blocks for a second

	_eventQueue.pop();
	auto& handler = _eventHandlers.at(event.element->getEventName());
	bool success = handler->handle(*event.element);
	event.result->set_value(success);

	if (!success) std::cout << "Failed to handle event of type: " << event.element->getEventName() << std::endl;
}

void InputController::enterPressedThread()
{
	if (_consoleMode == ConsoleDevice::INPUT) return;

	if (_device->awaitInputMode(std::chrono::milliseconds(1)) && _consoleMode == ConsoleDevice::OUTPUT)
		onNewEvent(std::make_shared<EnterPressedInputEvent>()); // here wait that the event is completed
}

std::string InputController::readLine(bool secret)
{
	std::string readLine;
	bool readSuccess = _device->read(readLine, secret);
	if (!readSuccess) return "";

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
