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

using ::testing::_;

class InputControllerTests : public testing::Test
{
protected:
	void SetUp() override
	{
		_consoleDeviceMock = std::make_shared<ConsoleDeviceMock>();
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
	}

	std::shared_ptr<ConsoleDeviceMock> _consoleDeviceMock;
	std::shared_ptr<ghost::internal::InputController> _inputController;

public:
	void commandCallback(const std::string& commandLine) {}
	void modeCallback(ghost::internal::ConsoleDevice::ConsoleMode mode) {}
};

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

}

TEST_F(InputControllerTests, Test_InputController_deviceStartCalledOnce_When_controllerStartCalledTwice)
{

}

TEST_F(InputControllerTests, Test_InputController_deviceStopCalledOnce_When_controllerStopCalledOnce)
{

}

TEST_F(InputControllerTests, Test_InputController_deviceStopCalledTwice_When_controllerStopCalledTwice)
{

}

/* Checks the prompt function */

TEST_F(InputControllerTests, Test_InputController_deviceWriteIsCalled_When_printPromptIsCalled)
{

}

TEST_F(InputControllerTests, Test_InputController_promptMatchesConfiguration_When_unchanged)
{

}

TEST_F(InputControllerTests, Test_InputController_promptMatchesConfiguration_When_textIsChanged)
{

}

/* Checks InputController::ReadLine */

TEST_F(InputControllerTests, Test_InputController_deviceReadIsCalled_When_controllerReadLineIsCalled)
{

}

/* Checks the command callbacks */

TEST_F(InputControllerTests, Test_InputController_commandCallbackIsCalled_When_onNewInputIsCalled)
{

}

TEST_F(InputControllerTests, Test_InputController_newCommandCallbackIsCalled_When_commandCallbackIsUpdated)
{

}

/* Check that the command callback is called */

TEST_F(InputControllerTests, Test_InputController_modeCallbackIsCalled_When_modeIsSwitched)
{

}

/* Checks the workflow of getLine */

TEST_F(InputControllerTests, Test_InputController_deviceConsoleModeSwitchesToInput_When_getLineIsCalled)
{

}

TEST_F(InputControllerTests, Test_InputController_deviceConsoleModeSwitchesBackToOutput_When_getLineIsCalledAndCurrentStateIsOutput)
{

}

TEST_F(InputControllerTests, Test_InputController_inputLineIsGotten_When_getLineIsCalled)
{

}

/* Checks that events are executed when the controller is started */

TEST_F(InputControllerTests, Test_InputController_eventIsExecuted_When_newEventIsAddedAndControllerIsStarted)
{

}

TEST_F(InputControllerTests, Test_InputController_eventIsNotExecuted_When_newEventIsAddedAndControllerIsNotStarted)
{

}

TEST_F(InputControllerTests, Test_InputController_eventIsNotExecuted_When_newEventIsAddedAndControllerIsStopped)
{

}

/* Checks the workflow of enterPressedThread */

TEST_F(InputControllerTests, Test_InputController_deviceAwaitInputModeIsCalled_When_inputModeIsOuputAndConsoleIsStarted)
{

}

TEST_F(InputControllerTests, Test_InputController_deviceAwaitInputModeIsNotCalled_When_inputModeIsOuputAndConsoleIsStopped)
{

}

TEST_F(InputControllerTests, Test_InputController_deviceAwaitInputModeIsNotCalled_When_inputModeIsInput)
{

}

TEST_F(InputControllerTests, Test_InputController_deviceSetConsoleModeInputIsCalled_When_deviceAwaitInputModeReturnedTrue)
{

}

/* Checks the workflow of InputModeEventHandler */

TEST_F(InputControllerTests, Test_InputController_deviceReadIsCalled_When_deviceAwaitInputModeReturnedTrue)
{

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
