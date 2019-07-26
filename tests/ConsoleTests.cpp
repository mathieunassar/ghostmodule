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

#include <gtest/gtest.h>

using ::testing::_;

class ConsoleTests : public testing::Test
{
protected:
	void SetUp() override
	{
	}

	void TearDown() override
	{
	}
};

/* Checks the initialization of the controller */

TEST_F(ConsoleTests, Test_Console_doesNotHang_When_startedAndStopped)
{

}

TEST_F(ConsoleTests, Test_Console_commandIsReceived_When_inputIsProvided)
{

}

TEST_F(ConsoleTests, Test_Console_commandIsReceivedOnlyOnce_When_inputIsProvided)
{

}

TEST_F(ConsoleTests, Test_Console_PromptIsUpdated)
{

}

TEST_F(ConsoleTests, Test_Console_writeIsForwardedToConsole)
{

}

TEST_F(ConsoleTests, Test_Console_getLineFlushesBeforeReading)
{

}

TEST_F(ConsoleTests, Test_Console_customCommandCallbackCanBeSet)
{

}
