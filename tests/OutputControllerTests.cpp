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

#include "../src/module/OutputController.hpp"

#include <gtest/gtest.h>

#include "ConsoleDeviceMock.hpp"

class OutputControllerTests : public testing::Test
{
protected:
	void SetUp() override
	{
		_consoleDeviceMock = std::make_shared<ConsoleDeviceMock>();
		_outputController = std::make_shared<ghost::internal::OutputController>(nullptr);
	}

	void TearDown() override
	{
		_consoleDeviceMock.reset();
	}

	void startController()
	{
		if (_outputController)
		{
			_outputController->start();
		}
	}

	void stopController()
	{
		if (_outputController)
		{
			_outputController->stop();
		}
	}

	std::shared_ptr<ConsoleDeviceMock> _consoleDeviceMock;
	std::shared_ptr<ghost::internal::OutputController> _outputController;
};

/* Checks the initialization of the controller */

TEST_F(OutputControllerTests, Test_OutputController_doesNotHang_When_startedAndStopped)
{

}

TEST_F(OutputControllerTests, Test_OutputController_modeIsOutput_When_ConstructorIsCalled)
{

}

TEST_F(OutputControllerTests, Test_OutputController_redirectsStdCout)
{

}

TEST_F(OutputControllerTests, Test_OutputController_doesNotRedirectStdCout_When_OptionIsDisabled)
{

}

TEST_F(OutputControllerTests, Test_OutputController_deviceWriteIsNotCalled_When_ControllerNotStartedAndWriteIsCalled)
{

}

TEST_F(OutputControllerTests, Test_OutputController_deviceWriteIsCalled_When_ControllerIsStartedAndWriteIsCalled)
{

}

TEST_F(OutputControllerTests, Test_OutputController_deviceWriteIsNotCalled_When_ControllerIsDisabled)
{

}

TEST_F(OutputControllerTests, Test_OutputController_deviceWriteIsCalled_When_ControllerIsEnabledAgain)
{

}

TEST_F(OutputControllerTests, Test_OutputController_flushSwapsQueues)
{
	// call flush when disabled, write another time, then enable and check that the last write is not called
}
