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
#include "../connection/ConnectionTestUtils.hpp"
#include "../src/connected_module/RemoteConsole.hpp"
#include "../src/connected_module/RemoteHandler.hpp"
#include <future>

class CommandLineInterpreterMock : public ghost::CommandLineInterpreter
{
public:
	MOCK_METHOD2(execute, bool(const std::string& commandLine, const ghost::CommandExecutionContext& context));
	MOCK_METHOD2(execute,
		     bool(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context));
	MOCK_METHOD2(registerCommand, void(std::shared_ptr<ghost::Command> command,
					   const std::list<std::shared_ptr<ghost::PermissionEntity>>& permissions));
	MOCK_CONST_METHOD2(printHelp, void(std::ostream& stream, const std::shared_ptr<ghost::Session>& session));
};

class ConnectedModuleTests : public testing::Test
{
protected:
	void SetUp() override
	{
		auto config = ghost::ConnectionConfiguration();
		config.setOperationBlocking(false); // otherwise the writer will wait until the sink is empty
		_client = std::make_shared<ClientMock>(config);
		_console = std::make_shared<ghost::internal::RemoteConsole>(_client);
		_interpreter = std::make_shared<CommandLineInterpreterMock>();
	}

	void TearDown() override
	{
	}

	void feedClient(const std::string& text)
	{
		auto msg = google::protobuf::StringValue::default_instance();
		msg.set_value(text);
		google::protobuf::Any anyMsg;
		anyMsg.PackFrom(msg);
		_client->pushMessage(anyMsg);
	}

	std::shared_ptr<ghost::internal::RemoteConsole> _console;
	std::shared_ptr<ClientMock> _client;
	std::shared_ptr<ghost::CommandLineInterpreter> _interpreter;
	std::shared_ptr<ghost::internal::RemoteHandler> _remoteHandler;

	static const std::string TEST_CONSOLE_ENTRY;
};

const std::string ConnectedModuleTests::TEST_CONSOLE_ENTRY = "TEST_NAME";

// RemoteConsole

TEST_F(ConnectedModuleTests, test_RemoteConsole_getLineReadsFromClient_When_messageNotReceivedYet)
{
	std::future<std::string> getLineFuture = std::async(std::launch::async,
														[&]{ return _console->getLine(); });

	// check that getline did not return already
	auto futureStatus = getLineFuture.wait_for(std::chrono::milliseconds(50));
	ASSERT_TRUE(futureStatus != std::future_status::ready);

	// feed the client
	feedClient(TEST_CONSOLE_ENTRY);

	futureStatus = getLineFuture.wait_for(std::chrono::milliseconds(100));
	ASSERT_TRUE(futureStatus == std::future_status::ready);
	std::string res = getLineFuture.get();
	ASSERT_TRUE(res == TEST_CONSOLE_ENTRY);
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_getLineReadsFromClient_When_messageAlreadyReceived)
{
	feedClient(TEST_CONSOLE_ENTRY);
	std::future<std::string> getLineFuture = std::async(std::launch::async,
														[&]{ return _console->getLine(); });
	auto futureStatus = getLineFuture.wait_for(std::chrono::milliseconds(50));
	ASSERT_TRUE(futureStatus == std::future_status::ready);
	std::string res = getLineFuture.get();
	ASSERT_TRUE(res == TEST_CONSOLE_ENTRY);
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_writeSendsToClient)
{
	_console->write(TEST_CONSOLE_ENTRY);
	google::protobuf::Any anyMsg;
	bool messageSent = _client->getMessage(anyMsg, std::chrono::milliseconds(50));
	ASSERT_TRUE(messageSent);
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_hasCommandsReturnsTrue_When_clientReceivedMessage)
{
	feedClient(TEST_CONSOLE_ENTRY);
	ASSERT_TRUE(_console->hasCommands());
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_hasCommandsReturnsFalse_When_nothingWasSent)
{
	ASSERT_FALSE(_console->hasCommands());
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_getCommandReturnsACommand_When_commandWasAvailable)
{
	feedClient(TEST_CONSOLE_ENTRY);
	auto command = _console->getCommand();
	ASSERT_TRUE(command == TEST_CONSOLE_ENTRY);
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_getCommandThrows_When_noCommandWasAvailable)
{
	bool threw = false;
	try
	{
		auto command = _console->getCommand();
	}
	catch (std::exception e)
	{
		threw = true;
	}
	ASSERT_TRUE(threw);
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_callbackIsCalled_When_callbackWasSetAndCommandIsSent)
{
	bool callbackWasCalled = false;
	std::string result;
	_console->setCommandCallback([&](const std::string& s){ callbackWasCalled = true; result = s; });
	feedClient(TEST_CONSOLE_ENTRY);
	ASSERT_TRUE(callbackWasCalled);
	ASSERT_TRUE(result == TEST_CONSOLE_ENTRY);
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_stopCallsClientStop)
{
	EXPECT_CALL(*_client, stop()).Times(1);
	_console->stop();
}

// RemoteHandler

TEST_F(ConnectedModuleTests, test_RemoteHandler_isActive_When_clientIsRunning)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_isActive_When_clientIsNotRunningButStateIsExecuting)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_isNotActive_When_clientIsNotRunningAndStateIsNotExecuting)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_executes_When_oneCommandIsProvided)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_executesTwoCommands_WhenTwoCommandsAreProvided)
{
	ASSERT_TRUE(false);
}

// RemoteControlClient

TEST_F(ConnectedModuleTests, test_RemoteControlClient_doesNotStart_When_connectionManagerDoesNotCreate)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_doesNotStart_When_consoleIsNotSet)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_doesNotStart_When_clientDoesNotStart)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_starts_When_ok)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_callsInterpreter_When_localConsoleCommandIsSent)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_callsClientWriter_When_remoteConsoleCommandIsSent)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_writesToConsole_When_clientReceivesMessage)
{
	ASSERT_TRUE(false);
}

// RemoteAccessServer

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_doesNotStart_When_connectionMangerDoesNotCreate)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_doesNotStart_When_serverDoesNotStart)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_starts_When_oneConfigurationIsSet)
{
	ASSERT_TRUE(false);
}

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_starts_When_twoConfigurationsAreSet)
{
	ASSERT_TRUE(false);
}
