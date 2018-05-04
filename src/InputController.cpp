#include "../include/internal/InputController.hpp"

#include <iostream>
#include <future>
#include <chrono>

#include "../include/internal/EnterPressedInputEvent.hpp"
#include "../include/internal/LineRequestInputEvent.hpp"
#include "../include/internal/InputModeInputEvent.hpp"

using namespace Ghost::internal;

InputController::InputController(std::shared_ptr<ConsoleDevice> device,
	ConsoleDevice::ConsoleMode initialMode,
	std::function<void(const std::string&)> cmdCallback,
	std::function<void(ConsoleDevice::ConsoleMode)> modeCallback)
	: _threadEnable(false)
	, _device(device)
	, _prompt("> ")
	, _consoleMode(initialMode)
	, _inputMode(DISCRETE)
	, _commandCallback(cmdCallback)
	, _modeCallback(modeCallback)
{
	registerEventHandlers();

	_device->setConsoleMode(initialMode);
}

void InputController::setPrompt(const std::string& prompt)
{
	_prompt = prompt;
}

void InputController::setInputMode(InputMode mode)
{
	_inputMode = mode;
}

void InputController::start()
{
	if (!_threadEnable)
	{
		_device->start();

		_threadEnable = true;
		_inputThread = std::thread(&InputController::inputListenerThread, this);
		_enterPressedThread = std::thread(&InputController::enterPressedThread, this);
	}
}

void InputController::stop()
{
	_device->stop();

	_threadEnable = false;
	if (_inputThread.joinable())
		_inputThread.join();

	if (_enterPressedThread.joinable())
		_enterPressedThread.join();
}

std::string InputController::getLine()
{
	_explicitInput = nullptr;

	auto& promise = onNewEvent(std::make_shared<LineRequestInputEvent>()); // here wait that the event is completed
	promise.get_future().wait();

	if (_explicitInput)
	{
		return *_explicitInput;
	}
	return "";
}

void InputController::printPrompt() const
{
	printf(_prompt.c_str());
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
	while (_threadEnable)
	{
		QueueElement<std::shared_ptr<InputEvent>> event;
		if (!_eventQueue.tryPop(std::chrono::milliseconds(1000), event)) // trypop only blocks for a second
		{
			continue;
		}
		
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
	while (_threadEnable)
	{
		if (_consoleMode == ConsoleDevice::INPUT)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		if (_device->awaitInputMode() && _consoleMode == ConsoleDevice::OUTPUT)
		{
			auto& promise = onNewEvent(std::make_shared<EnterPressedInputEvent>()); // here wait that the event is completed
			promise.get_future().wait();
		}
	}
}

std::string InputController::readLine()
{
	std::promise<std::string> p;
	auto future = p.get_future();
	
	std::thread t([&p] {
		std::string str;
		std::getline(std::cin, str);
		p.set_value(str);
	});
	
	auto status = future.wait_for(std::chrono::seconds(0));
	while (status != std::future_status::ready && _threadEnable)
	{
		status = future.wait_for(std::chrono::seconds(1));
	}

	if (status == std::future_status::ready)
	{
		t.join();

		return future.get();
	}
	else // the program must exit, t is detached and will die peacefully without triggering exceptions on this one (->legal?? update yes but does not free allocated resources -> mem leak)
	{
		t.detach();
		return "";
	}
}

InputController::InputMode InputController::getInputMode() const
{
	return _inputMode;
}

void InputController::onNewInput(const std::string& input)
{
	_commandCallback(input);
}

std::promise<bool>& InputController::onNewEvent(std::shared_ptr<InputEvent> event)
{
	QueueElement<std::shared_ptr<InputEvent>> element;
	element.element = event;
	element.result = std::make_shared<std::promise<bool>>();
	_eventQueue.push(element);
	return *element.result;
}

void InputController::setLineRequestResult(const std::string& line)
{
	_explicitInput = std::make_shared<std::string>(line);
}

ConsoleDevice::ConsoleMode InputController::getConsoleMode() const
{
	return _consoleMode;
}
