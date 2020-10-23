/*
 * Copyright 2020 Mathieu Nassar
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

#include <gtest/gtest.h>

#include "../src/module/Console.hpp"
#include "ConsoleDeviceMock.hpp"

using ::testing::_;

class ConsoleTests : public testing::Test
{
protected:
	void SetUp() override
	{
		_consoleDeviceMock = std::make_shared<ConsoleDeviceMock>();
		_callbackCount = 0;
		_lastCommandReceived.clear();

		EXPECT_CALL(*_consoleDeviceMock, awaitInputMode(_))
		    .Times(testing::AnyNumber())
		    .WillRepeatedly(testing::Return(false));
		EXPECT_CALL(*_consoleDeviceMock, start()).Times(testing::AnyNumber());
		EXPECT_CALL(*_consoleDeviceMock, stop()).Times(testing::AnyNumber());
		EXPECT_CALL(*_consoleDeviceMock, setConsoleMode(_)).Times(testing::AnyNumber());
		EXPECT_CALL(*_consoleDeviceMock, write(">")).Times(testing::AnyNumber());

		_console = std::make_shared<ghost::internal::Console>(_consoleDeviceMock, nullptr, false);
	}

	void TearDown() override
	{
		if (_console) _console->stop();

		_console.reset();
		_consoleDeviceMock.reset();
	}

	void gotoInputIsProvided(bool commandExpected = true)
	{
		EXPECT_CALL(*_consoleDeviceMock, awaitInputMode(_))
		    .Times(testing::AnyNumber())
		    .WillOnce(testing::Return(true))
		    .WillRepeatedly(testing::Return(false));

		EXPECT_CALL(*_consoleDeviceMock, read(_, false))
		    .Times(1)
		    .WillRepeatedly(
			testing::DoAll(testing::SetArgReferee<0>(TEST_COMMAND_LINE), testing::Return(true)));

		runConsole(commandExpected);
	}

	void runConsole(bool commandExpected = true)
	{
		_console->start();

		if (commandExpected)
		{
			auto now = std::chrono::steady_clock::now();
			auto deadline = now + std::chrono::milliseconds(500);
			while (!_console->hasCommands() && now < deadline)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				now = std::chrono::steady_clock::now();
			}
		}
		else
			// wait to let internal threads reach the test points
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	std::shared_ptr<ConsoleDeviceMock> _consoleDeviceMock;
	std::shared_ptr<ghost::internal::Console> _console;

	int _callbackCount;
	std::string _lastCommandReceived;

	static const std::string TEST_COMMAND_LINE;
	static const std::string TEST_WRITE_LINE;
	static const std::string TEST_WRITE_LINE2;

public:
	void commandCallback(const std::string& command)
	{
		_callbackCount++;
		_lastCommandReceived = command;
	}
};

const std::string ConsoleTests::TEST_COMMAND_LINE = "TEST_COMMAND_LINE";
const std::string ConsoleTests::TEST_WRITE_LINE = "TEST_WRITE_LINE";
const std::string ConsoleTests::TEST_WRITE_LINE2 = "TEST_WRITE_LINE2";

/* Checks the initialization of the controller */

TEST_F(ConsoleTests, Test_Console_doesNotHang_When_startedAndStopped)
{
	_console->start();
}

TEST_F(ConsoleTests, Test_Console_commandIsReceived_When_inputIsProvided)
{
	gotoInputIsProvided();
	ASSERT_TRUE(_console->hasCommands());

	ASSERT_TRUE(_console->hasCommands());
	ASSERT_TRUE(_console->getCommand() == TEST_COMMAND_LINE);
}

TEST_F(ConsoleTests, Test_Console_commandIsReceivedOnlyOnce_When_inputIsProvided)
{
	gotoInputIsProvided();
	ASSERT_TRUE(_console->hasCommands());
	_console->getCommand();

	ASSERT_FALSE(_console->hasCommands());
	bool exceptionCaught = false;
	try
	{
		_console->getCommand();
	}
	catch (std::exception e)
	{
		exceptionCaught = true;
	}
	ASSERT_TRUE(exceptionCaught);
}

TEST_F(ConsoleTests, Test_Console_getLineFlushesBeforeReading)
{
	_console->write(TEST_WRITE_LINE);
	_console->write(TEST_WRITE_LINE2);

	bool ready4GetLine = false;

	{
		testing::InSequence seq;
		// the first call to write will take 100ms so that "_console->getLine()" reaches flush before all write
		// lines are processed
		EXPECT_CALL(*_consoleDeviceMock, write(TEST_WRITE_LINE))
		    .Times(1)
		    .WillOnce(testing::InvokeWithoutArgs([&] {
			    ready4GetLine = true;
			    std::this_thread::sleep_for(std::chrono::milliseconds(100));
			    return true;
		    }));
		EXPECT_CALL(*_consoleDeviceMock, write(TEST_WRITE_LINE2)).Times(1);

		EXPECT_CALL(*_consoleDeviceMock, read(_, false)).Times(1);
	}

	_console->start(); // necessary to be before getLine, otherwise flush won't flush

	std::thread t([this, &ready4GetLine] {
		while (!ready4GetLine) std::this_thread::sleep_for(std::chrono::milliseconds(1));
		(void)_console->getLine();
	});

	t.join();
}

TEST_F(ConsoleTests, Test_Console_customCommandCallbackCanBeSet)
{
	_console->setCommandCallback(std::bind(&ConsoleTests::commandCallback, this, std::placeholders::_1));
	gotoInputIsProvided(false);

	ASSERT_FALSE(_console->hasCommands());

	ASSERT_TRUE(_callbackCount == 1);
	ASSERT_TRUE(_lastCommandReceived == TEST_COMMAND_LINE);
}
