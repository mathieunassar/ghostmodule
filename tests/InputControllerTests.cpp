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

#include "../src/module/InputController.hpp"

#include <gtest/gtest.h>

#include "ConsoleDeviceMock.hpp"
#include "../src/module/LineRequestInputEvent.hpp"

using ::testing::_;

class InputControllerTests : public testing::Test
{
protected:
	void SetUp() override
	{
		_consoleDeviceMock = std::make_shared<ConsoleDeviceMock>();
		_commandCallbackCallCounter = 0;
		_modeCallbackCallCounter = 0;
	}

	void TearDown() override
	{
		if (_inputController)
		{
			EXPECT_CALL(*_consoleDeviceMock, stop()).Times(1);
			_inputController.reset();
		}

		_consoleDeviceMock.reset();
	}

	void setupInputController()
	{
		ghost::internal::ConsoleDevice::ConsoleMode initialMode = ghost::internal::ConsoleDevice::OUTPUT;
		EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(initialMode)).Times(1);

		_inputController = std::make_shared<ghost::internal::InputController>(_consoleDeviceMock, initialMode,
			std::bind(&InputControllerTests::commandCallback, this, std::placeholders::_1),
			std::bind(&InputControllerTests::modeCallback, this, std::placeholders::_1));

		EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(_)).Times(testing::AnyNumber());
	}

	void startController()
	{
		if (_inputController)
		{
			EXPECT_CALL(*_consoleDeviceMock, start()).Times(1);
			_inputController->start();
		}
	}

	void stopController()
	{
		if (_inputController)
		{
			EXPECT_CALL(*_consoleDeviceMock, stop()).Times(1);
			_inputController->stop();
		}
	}

	std::shared_ptr<ConsoleDeviceMock> _consoleDeviceMock;
	std::shared_ptr<ghost::internal::InputController> _inputController;

	int _commandCallbackCallCounter;
	int _modeCallbackCallCounter;

	static const std::string TEST_PROMPT_FORMAT;
	static const std::string TEST_READ_LINE;

public:
	void commandCallback(const std::string& commandLine)
	{
		_commandCallbackCallCounter++;
	}
	
	void modeCallback(ghost::internal::ConsoleDevice::ConsoleMode mode)
	{
		_modeCallbackCallCounter++;
	}
};

const std::string InputControllerTests::TEST_PROMPT_FORMAT = "TEST_FORMAT";
const std::string InputControllerTests::TEST_READ_LINE = "TEST_READ_LINE";

/* Checks the initialization of the controller */

TEST_F(InputControllerTests, Test_InputController_InitialConsoleStateIsCorrect_When_constructorInitialized)
{
	setupInputController();
}

TEST_F(InputControllerTests, Test_InputController_getInputModeIsDiscrete_When_constructorInitialized)
{
	setupInputController();

	ASSERT_TRUE(_inputController->getInputMode() == ghost::InputMode::DISCRETE);
}

/* Checks setters and getters */

TEST_F(InputControllerTests, Test_InputController_setInputMode_When_ok)
{
	setupInputController();

	ghost::InputMode expectedMode = ghost::InputMode::SEQUENTIAL;
	_inputController->setInputMode(expectedMode);

	ASSERT_TRUE(_inputController->getInputMode() == expectedMode);
}

TEST_F(InputControllerTests, Test_InputController_deviceSetConsoleModeIsCalled_When_controllerSwitchModeIsCalled)
{
	setupInputController();

	ghost::internal::ConsoleDevice::ConsoleMode expectedMode = ghost::internal::ConsoleDevice::INPUT;
	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(expectedMode)).Times(1);

	_inputController->switchConsoleMode(expectedMode);
	ASSERT_TRUE(_inputController->getConsoleMode() == expectedMode);
}

/* Checks start and stop workflows */

TEST_F(InputControllerTests, Test_InputController_deviceStartCalledOnce_When_controllerStartCalledOnce)
{
	setupInputController();
	startController();
}

TEST_F(InputControllerTests, Test_InputController_deviceStartCalledOnce_When_controllerStartCalledTwice)
{
	setupInputController();
	startController();

	_inputController->start();
}

TEST_F(InputControllerTests, Test_InputController_deviceStopCalledOnce_When_controllerStopCalledOnce)
{
	setupInputController();
	startController();

	stopController();
}

TEST_F(InputControllerTests, Test_InputController_deviceStopCalledTwice_When_controllerStopCalledTwice)
{
	setupInputController();
	startController();
	stopController();

	EXPECT_CALL(*_consoleDeviceMock, stop()).Times(1);
	_inputController->stop();
}

/* Checks the prompt function */

TEST_F(InputControllerTests, Test_InputController_deviceWriteIsCalled_When_printPromptIsCalled)
{
	setupInputController();

	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(1);
	_inputController->printPrompt();
}

TEST_F(InputControllerTests, Test_InputController_promptMatchesConfiguration_When_unchanged)
{
	setupInputController();

	std::string expectedWrite = _inputController->getPrompt().str();
	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(0);
	EXPECT_CALL(*_consoleDeviceMock, write(expectedWrite)).Times(1);
	_inputController->printPrompt();
}

TEST_F(InputControllerTests, Test_InputController_promptMatchesConfiguration_When_textIsChanged)
{
	setupInputController();

	_inputController->getPrompt().setFormat(TEST_PROMPT_FORMAT);

	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(0);
	EXPECT_CALL(*_consoleDeviceMock, write(TEST_PROMPT_FORMAT)).Times(1);
	_inputController->printPrompt();
}

/* Checks InputController::ReadLine */

TEST_F(InputControllerTests, Test_InputController_deviceReadIsCalled_When_controllerReadLineIsCalled)
{
	setupInputController();

	EXPECT_CALL(*_consoleDeviceMock, read(_)).Times(1).WillRepeatedly(testing::DoAll(
		testing::SetArgReferee<0>(TEST_READ_LINE),
		testing::Return(true)));

	auto readLine = _inputController->readLine();
	ASSERT_TRUE(readLine == TEST_READ_LINE);
}

/* Checks the command callbacks */

TEST_F(InputControllerTests, Test_InputController_commandCallbackIsCalled_When_onNewInputIsCalled)
{
	setupInputController();

	_inputController->onNewInput(TEST_READ_LINE);
	ASSERT_TRUE(_commandCallbackCallCounter == 1);
}

TEST_F(InputControllerTests, Test_InputController_newCommandCallbackIsCalled_When_commandCallbackIsUpdated)
{
	int callbackCounter = 0;
	std::function<void(const std::string&)> callback([&](const std::string&) { callbackCounter++; return true; });

	setupInputController();
	_inputController->setCommandCallback(callback);

	_inputController->onNewInput(TEST_READ_LINE);
	ASSERT_TRUE(callbackCounter == 1);
	ASSERT_TRUE(_commandCallbackCallCounter == 0);
}

/* Check that the command callback is called */

TEST_F(InputControllerTests, Test_InputController_modeCallbackIsCalled_When_modeIsSwitched)
{
	setupInputController();

	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(_)).Times(1);
	_inputController->switchConsoleMode(ghost::internal::ConsoleDevice::INPUT);
	ASSERT_TRUE(_modeCallbackCallCounter == 1);
}

/* Checks the workflow of getLine */

TEST_F(InputControllerTests, Test_InputController_deviceConsoleModeSwitchesToInput_When_getLineIsCalled)
{
	setupInputController();
	startController();

	// to read
	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(ghost::internal::ConsoleDevice::INPUT)).Times(1);
	// while setting it back
	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(ghost::internal::ConsoleDevice::OUTPUT)).Times(1);
	_inputController->getLine();
}

TEST_F(InputControllerTests, Test_InputController_deviceConsoleModeSwitchesBackToInput_When_getLineIsCalledAndCurrentStateIsInput)
{
	setupInputController();

	// test condition is INPUT
	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(_)).Times(1);
	_inputController->switchConsoleMode(ghost::internal::ConsoleDevice::INPUT);

	startController();

	// to read and set it back
	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(ghost::internal::ConsoleDevice::INPUT)).Times(2);
	_inputController->getLine();
}

TEST_F(InputControllerTests, Test_InputController_inputLineIsGotten_When_getLineIsCalled)
{
	setupInputController();
	startController();

	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(_)).Times(2);
	EXPECT_CALL(*_consoleDeviceMock, read(_)).Times(1).WillRepeatedly(testing::DoAll(
		testing::SetArgReferee<0>(TEST_READ_LINE),
		testing::Return(true)));

	std::string gottenLine = _inputController->getLine();
	ASSERT_TRUE(gottenLine == TEST_READ_LINE);
}

/* Checks that events are executed when the controller is started */

TEST_F(InputControllerTests, Test_InputController_eventIsExecuted_When_newEventIsAddedAndControllerIsStarted)
{
	setupInputController();
	startController();

	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(_)).Times(2);
	auto promise = _inputController->onNewEvent(std::make_shared<ghost::internal::LineRequestInputEvent>());
	auto future = promise->get_future();
	future.wait();
	bool testResult = future.get();
	ASSERT_TRUE(testResult);
}

TEST_F(InputControllerTests, Test_InputController_eventIsNotExecuted_When_newEventIsAddedAndControllerIsNotStarted)
{
	setupInputController();

	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(_)).Times(0);
	auto promise = _inputController->onNewEvent(std::make_shared<ghost::internal::LineRequestInputEvent>());
	auto future = promise->get_future();
	ASSERT_TRUE(future.wait_for(std::chrono::seconds(1)) == std::future_status::timeout);
}

TEST_F(InputControllerTests, Test_InputController_eventIsNotExecuted_When_newEventIsAddedAndControllerIsStopped)
{
	setupInputController();
	startController();
	stopController();

	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(_)).Times(0);
	auto promise = _inputController->onNewEvent(std::make_shared<ghost::internal::LineRequestInputEvent>());
	auto future = promise->get_future();
	ASSERT_TRUE(future.wait_for(std::chrono::seconds(1)) == std::future_status::timeout);
}

/* Checks the workflow of enterPressedThread */

TEST_F(InputControllerTests, Test_InputController_deviceAwaitInputModeIsCalled_When_inputModeIsOuputAndConsoleIsStarted)
{
	setupInputController();

	EXPECT_CALL(*_consoleDeviceMock, awaitInputMode())
		.Times(testing::AnyNumber())
		.WillRepeatedly(testing::Return(false));

	startController();

	// wait to let internal threads reach the test points
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

TEST_F(InputControllerTests, Test_InputController_deviceAwaitInputModeIsNotCalled_When_inputModeIsOuputAndConsoleIsStopped)
{
	setupInputController();

	startController();
	stopController();

	EXPECT_CALL(*_consoleDeviceMock, awaitInputMode())
		.Times(0);

	// wait to let internal threads reach the test points
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

TEST_F(InputControllerTests, Test_InputController_deviceAwaitInputModeIsNotCalled_When_inputModeIsInput)
{
	setupInputController();

	EXPECT_CALL(*_consoleDeviceMock, awaitInputMode())
		.Times(0);

	_inputController->switchConsoleMode(ghost::internal::ConsoleDevice::INPUT);
	startController();

	// wait to let internal threads reach the test points
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

TEST_F(InputControllerTests, Test_InputController_deviceSetConsoleModeInputIsCalled_When_deviceAwaitInputModeReturnedTrue)
{
	setupInputController();

	EXPECT_CALL(*_consoleDeviceMock, awaitInputMode())
		.Times(testing::AnyNumber())
		.WillOnce(testing::Return(true))
		.WillRepeatedly(testing::Return(false));
	EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(ghost::internal::ConsoleDevice::INPUT)).Times(1);

	startController();

	// wait to let internal threads reach the test points
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

/* Checks the workflow of InputModeEventHandler */

TEST_F(InputControllerTests, Test_InputController_deviceReadIsCalled_When_deviceAwaitInputModeReturnedTrue)
{
	setupInputController();

	EXPECT_CALL(*_consoleDeviceMock, awaitInputMode())
		.Times(testing::AnyNumber())
		.WillOnce(testing::Return(true))
		.WillRepeatedly(testing::Return(false));
	EXPECT_CALL(*_consoleDeviceMock, read(_)).Times(1);

	startController();

	// wait to let internal threads reach the test points
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

TEST_F(InputControllerTests, Test_InputController_consoleModeSwitchesToOutput_When_inputModeEventReceivesAnEmptyLine)
{

}

TEST_F(InputControllerTests, Test_InputController_consoleModeSwitchesToOutput_When_inputModeEventReceivesANewCommand)
{

}

TEST_F(InputControllerTests, Test_InputController_commandCallbackIsCalled_When_inputModeEventReceivesANewCommand)
{

}

TEST_F(InputControllerTests, Test_InputController_consoleModeSwitchesBackToInput_When_inputModeEventReceivesANewCommandAndInputModeIsSequential)
{

}
