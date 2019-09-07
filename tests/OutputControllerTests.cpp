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

using ::testing::_;

class OutputControllerTests : public testing::Test
{
protected:
	void SetUp() override
	{
		_consoleDeviceMock = std::make_shared<ConsoleDeviceMock>();
	}

	void TearDown() override
	{
		if (_outputController)
			_outputController.reset();
		
		_consoleDeviceMock.reset();
	}

	void setupOutputController(bool redirect = true)
	{
		_outputController = std::make_shared<ghost::internal::OutputController>(_consoleDeviceMock, redirect);
	}

	void startController()
	{
		if (_outputController)
			_outputController->start();
	}

	void stopController()
	{
		if (_outputController)
			_outputController->stop();
	}

	void runController()
	{
		startController();

		// wait to let internal threads reach the test points
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	std::shared_ptr<ConsoleDeviceMock> _consoleDeviceMock;
	std::shared_ptr<ghost::internal::OutputController> _outputController;

	static const std::string TEST_WRITE_LINE;
	static const std::string TEST_WRITE_LINE2;
};

const std::string OutputControllerTests::TEST_WRITE_LINE = "TEST_WRITE_LINE";
const std::string OutputControllerTests::TEST_WRITE_LINE2 = "TEST_WRITE_LINE2";

/* Checks the initialization of the controller */

TEST_F(OutputControllerTests, Test_OutputController_doesNotHang_When_startedAndStopped)
{
	setupOutputController();
	startController();
}

TEST_F(OutputControllerTests, Test_OutputController_modeIsOutput_When_ConstructorIsCalled)
{
	setupOutputController();
	ASSERT_TRUE(_outputController->isEnabled());
}

TEST_F(OutputControllerTests, Test_OutputController_redirectsStdCout)
{
	setupOutputController();
	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(1);
	std::cout << TEST_WRITE_LINE;
	runController();
}

TEST_F(OutputControllerTests, Test_OutputController_doesNotRedirectStdCout_When_OptionIsDisabled)
{
	setupOutputController(false);
	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(0);
	std::cout << TEST_WRITE_LINE;
	runController();
}

TEST_F(OutputControllerTests, Test_OutputController_deviceWriteIsNotCalled_When_ControllerNotStartedAndWriteIsCalled)
{
	setupOutputController();
	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(0);
	_outputController->write(TEST_WRITE_LINE);
	std::this_thread::sleep_for(std::chrono::milliseconds(200)); // wait to see that write is never called
}

TEST_F(OutputControllerTests, Test_OutputController_deviceWriteIsCalled_When_ControllerIsStartedAndWriteIsCalled)
{
	setupOutputController();
	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(1);
	_outputController->write(TEST_WRITE_LINE);
	runController();
}

TEST_F(OutputControllerTests, Test_OutputController_deviceWriteIsNotCalled_When_ControllerIsDisabled)
{
	setupOutputController();
	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(0);
	_outputController->start();

	_outputController->disable();
	ASSERT_FALSE(_outputController->isEnabled());
	
	_outputController->write(TEST_WRITE_LINE);
	std::this_thread::sleep_for(std::chrono::milliseconds(200)); // wait to see that write is never called
}

TEST_F(OutputControllerTests, Test_OutputController_deviceWriteIsCalled_When_ControllerIsEnabledAgain)
{
	setupOutputController();
	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(0);
	_outputController->start();

	_outputController->disable();
	ASSERT_FALSE(_outputController->isEnabled());

	_outputController->write(TEST_WRITE_LINE);
	std::this_thread::sleep_for(std::chrono::milliseconds(200)); // wait to see that write is never called

	EXPECT_CALL(*_consoleDeviceMock, write(_)).Times(1);
	_outputController->enable();
	ASSERT_TRUE(_outputController->isEnabled());
	// wait to let internal threads reach the test points
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

TEST_F(OutputControllerTests, Test_OutputController_flushAndWrite)
{
	setupOutputController();
	_outputController->start();

	EXPECT_CALL(*_consoleDeviceMock, write(TEST_WRITE_LINE)).Times(1);
	EXPECT_CALL(*_consoleDeviceMock, write(TEST_WRITE_LINE2)).Times(1);

	_outputController->write(TEST_WRITE_LINE);
	_outputController->flush();
	_outputController->write(TEST_WRITE_LINE2);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
