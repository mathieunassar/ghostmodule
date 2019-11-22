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
		_client = std::make_shared<ClientMock>(ghost::ConnectionConfiguration());
		_console = std::make_shared<ghost::internal::RemoteConsole>(_client);
		_interpreter = std::make_shared<CommandLineInterpreterMock>();
	}

	void TearDown() override
	{
	}

	std::shared_ptr<ghost::internal::RemoteConsole> _console;
	std::shared_ptr<ghost::Client> _client;
	std::shared_ptr<ghost::CommandLineInterpreter> _interpreter;
	std::shared_ptr<ghost::internal::RemoteHandler> _remoteHandler;
};

// RemoteConsole

TEST_F(ConnectedModuleTests, test_RemoteConsole_getLineReadsFromClient_When_messageNotReceivedYet)
{
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_getLineReadsFromClient_When_messageAlreadyReceived)
{
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_writeSendsToClient)
{
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_hasCommandsReturnsTrue_When_clientReceivedMessage)
{
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_hasCommandsReturnsFalse_When_nothingWasSent)
{
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_getCommandReturnsACommand_When_commandWasAvailable)
{
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_getCommandThrows_When_noCommandWasAvailable)
{
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_callbackIsCalled_When_callbackWasSetAndCommandIsSent)
{
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_stopCallsClientStop)
{
}

// RemoteHandler

TEST_F(ConnectedModuleTests, test_RemoteHandler_isActive_When_clientIsRunning)
{
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_isActive_When_clientIsNotRunningButStateIsExecuting)
{
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_isNotActive_When_clientIsNotRunningAndStateIsNotExecuting)
{
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_executes_When_oneCommandIsProvided)
{
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_executesTwoCommands_WhenTwoCommandsAreProvided)
{
}

// RemoteControlClient

TEST_F(ConnectedModuleTests, test_RemoteControlClient_doesNotStart_When_connectionManagerDoesNotCreate)
{
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_doesNotStart_When_consoleIsNotSet)
{
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_doesNotStart_When_clientDoesNotStart)
{
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_starts_When_ok)
{
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_callsInterpreter_When_localConsoleCommandIsSent)
{
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_callsClientWriter_When_remoteConsoleCommandIsSent)
{
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_writesToConsole_When_clientReceivesMessage)
{
}

// RemoteAccessServer

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_doesNotStart_When_connectionMangerDoesNotCreate)
{
}

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_doesNotStart_When_serverDoesNotStart)
{
}

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_starts_When_oneConfigurationIsSet)
{
}

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_starts_When_twoConfigurationsAreSet)
{
}
