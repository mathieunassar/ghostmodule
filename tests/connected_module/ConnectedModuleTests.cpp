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
#include <future>
#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/module/StdoutLogger.hpp>
#include "../connection/ConnectionTestUtils.hpp"
#include "../src/connected_module/RemoteAccessServer.hpp"
#include "../src/connected_module/RemoteConsole.hpp"
#include "../src/connected_module/RemoteControlClient.hpp"
#include "../src/connected_module/RemoteHandler.hpp"

using testing::_;

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
		_configuration = ghost::ConnectionConfiguration();
		_configuration.setOperationBlocking(false); // otherwise the writer will wait until the sink is empty
		_client = std::make_shared<ClientMock>(_configuration);
		_console = std::make_shared<ghost::internal::RemoteConsole>(_client);
		_interpreter = std::make_shared<CommandLineInterpreterMock>();

		_connectionManager = ghost::ConnectionManager::create();
		_connectionManager->getConnectionFactory()->addClientRule<ClientMock>(_configuration);
		_connectionManager->getConnectionFactory()->addServerRule<ServerMock>(_configuration);
	}

	void TearDown() override
	{
	}

	void feedClient(const std::shared_ptr<ClientMock>& client, const std::string& text)
	{
		auto msg = google::protobuf::StringValue::default_instance();
		msg.set_value(text);
		google::protobuf::Any anyMsg;
		anyMsg.PackFrom(msg);
		client->pushMessage(anyMsg);
	}

	bool checkClientWriterWasCalled(const std::shared_ptr<ClientMock>& client, const std::string& expectedMessage)
	{
		google::protobuf::Any anyMsg;
		bool writeWasCalled = client->getMessage(anyMsg, std::chrono::milliseconds(100));
		if (!writeWasCalled) return false;

		google::protobuf::StringValue actualMsg;
		anyMsg.UnpackTo(&actualMsg);
		return (actualMsg.value() == expectedMessage);
	}

	void makeRemoteControlClient(std::shared_ptr<ghost::ConnectionManager> connectionManager)
	{
		_remoteControlClient = std::make_shared<ghost::internal::RemoteControlClient>(
		    _configuration, connectionManager, _interpreter, _console, ghost::StdoutLogger::create());
	}

	void makeRemoteAccessServer(size_t configurationCount)
	{
		std::vector<ghost::ConnectionConfiguration> confs;
		for (size_t i = 0; i <= configurationCount; ++i)
		{
			confs.push_back(_configuration);
		}
		_remoteAccessServer = std::make_shared<ghost::internal::RemoteAccessServer>(
		    confs, _connectionManager, _interpreter, ghost::StdoutLogger::create());
	}

	ghost::ConnectionConfiguration _configuration;
	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
	std::shared_ptr<ghost::internal::RemoteConsole> _console;
	std::shared_ptr<ClientMock> _client;
	std::shared_ptr<CommandLineInterpreterMock> _interpreter;
	std::shared_ptr<ghost::internal::RemoteHandler> _remoteHandler;
	std::shared_ptr<ghost::internal::RemoteControlClient> _remoteControlClient;
	std::shared_ptr<ghost::internal::RemoteAccessServer> _remoteAccessServer;

	static const std::string TEST_CONSOLE_ENTRY;
	static const std::string PREFIX_LOCAL_EXECUTION;
};

const std::string ConnectedModuleTests::TEST_CONSOLE_ENTRY = "TEST_NAME";
const std::string ConnectedModuleTests::PREFIX_LOCAL_EXECUTION = "local:";

// RemoteConsole

TEST_F(ConnectedModuleTests, test_RemoteConsole_getLineReadsFromClient_When_messageNotReceivedYet)
{
	std::future<std::string> getLineFuture = std::async(std::launch::async, [&] { return _console->getLine(); });

	// check that getline did not return already
	auto futureStatus = getLineFuture.wait_for(std::chrono::milliseconds(50));
	ASSERT_TRUE(futureStatus != std::future_status::ready);

	// feed the client
	feedClient(_client, TEST_CONSOLE_ENTRY);

	futureStatus = getLineFuture.wait_for(std::chrono::milliseconds(100));
	ASSERT_TRUE(futureStatus == std::future_status::ready);
	std::string res = getLineFuture.get();
	ASSERT_TRUE(res == TEST_CONSOLE_ENTRY);
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_getLineReadsFromClient_When_messageAlreadyReceived)
{
	feedClient(_client, TEST_CONSOLE_ENTRY);
	std::future<std::string> getLineFuture = std::async(std::launch::async, [&] { return _console->getLine(); });
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
	feedClient(_client, TEST_CONSOLE_ENTRY);
	ASSERT_TRUE(_console->hasCommands());
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_hasCommandsReturnsFalse_When_nothingWasSent)
{
	ASSERT_FALSE(_console->hasCommands());
}

TEST_F(ConnectedModuleTests, test_RemoteConsole_getCommandReturnsACommand_When_commandWasAvailable)
{
	feedClient(_client, TEST_CONSOLE_ENTRY);
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
	_console->setCommandCallback([&](const std::string& s) {
		callbackWasCalled = true;
		result = s;
	});
	feedClient(_client, TEST_CONSOLE_ENTRY);
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
	_remoteHandler = std::make_shared<ghost::internal::RemoteHandler>(_client, _interpreter);
	ASSERT_TRUE(_remoteHandler->isActive());
	_client->stop();
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_isActive_When_clientIsNotRunningButStateIsExecuting)
{
	EXPECT_CALL(*_interpreter, execute(TEST_CONSOLE_ENTRY, _))
	    .Times(testing::AnyNumber())
	    .WillOnce([](const std::string&, const ghost::CommandExecutionContext&) {
		    std::this_thread::sleep_for(std::chrono::milliseconds(50));
		    return true;
	    });
	_remoteHandler = std::make_shared<ghost::internal::RemoteHandler>(_client, _interpreter);

	_remoteHandler->commandCallback(TEST_CONSOLE_ENTRY);
	_client->stop();

	ASSERT_TRUE(_remoteHandler->isActive());
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_isNotActive_When_clientIsNotRunningAndStateIsNotExecuting)
{
	EXPECT_CALL(*_client, isRunning).WillRepeatedly(testing::Return(false));
	_remoteHandler = std::make_shared<ghost::internal::RemoteHandler>(_client, _interpreter);
	ASSERT_FALSE(_remoteHandler->isActive());
	_client->stop();
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_executes_When_oneCommandIsProvided)
{
	_remoteHandler = std::make_shared<ghost::internal::RemoteHandler>(_client, _interpreter);
	EXPECT_CALL(*_interpreter, execute(TEST_CONSOLE_ENTRY, _)).Times(1).WillRepeatedly(testing::Return(true));
	feedClient(_client, TEST_CONSOLE_ENTRY);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_client->stop();
}

TEST_F(ConnectedModuleTests, test_RemoteHandler_executesTwoCommands_WhenTwoCommandsAreProvided)
{
	_remoteHandler = std::make_shared<ghost::internal::RemoteHandler>(_client, _interpreter);
	EXPECT_CALL(*_interpreter, execute(TEST_CONSOLE_ENTRY, _)).Times(2).WillRepeatedly(testing::Return(true));
	feedClient(_client, TEST_CONSOLE_ENTRY);
	feedClient(_client, TEST_CONSOLE_ENTRY);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	_client->stop();
}

// RemoteControlClient

TEST_F(ConnectedModuleTests, test_RemoteControlClient_doesNotStart_When_connectionManagerDoesNotCreate)
{
	_remoteControlClient = std::make_shared<ghost::internal::RemoteControlClient>(
	    _configuration, ghost::ConnectionManager::create(), _interpreter, _console, ghost::StdoutLogger::create());
	ASSERT_FALSE(_remoteControlClient->start());
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_doesNotStart_When_consoleIsNotSet)
{
	_remoteControlClient = std::make_shared<ghost::internal::RemoteControlClient>(
	    _configuration, _connectionManager, _interpreter, nullptr, ghost::StdoutLogger::create());
	ASSERT_FALSE(_remoteControlClient->start());
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_doesNotStart_When_clientDoesNotStart)
{
	auto connectionManager = ghost::ConnectionManager::create();
	connectionManager->getConnectionFactory()->addClientRule<NotRunningClientMock>(_configuration);

	_remoteControlClient = std::make_shared<ghost::internal::RemoteControlClient>(
	    _configuration, connectionManager, _interpreter, _console, ghost::StdoutLogger::create());
	ASSERT_FALSE(_remoteControlClient->start());
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_starts_When_ok)
{
	makeRemoteControlClient(_connectionManager);
	ASSERT_TRUE(_remoteControlClient->start());
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_callsInterpreter_When_localConsoleCommandIsSent)
{
	EXPECT_CALL(*_interpreter, execute(TEST_CONSOLE_ENTRY, _)).Times(1);
	makeRemoteControlClient(_connectionManager);
	_remoteControlClient->start();

	// simulate that someone on the RC Client wrote in the local console
	feedClient(_client, PREFIX_LOCAL_EXECUTION + TEST_CONSOLE_ENTRY);
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_callsClientWriter_When_remoteConsoleCommandIsSent)
{
	auto connectionManagerMock = std::make_shared<ConnectionManagerMock>();
	makeRemoteControlClient(connectionManagerMock);
	auto remoteClient = std::make_shared<ClientMock>(_configuration);
	EXPECT_CALL(*connectionManagerMock, createClient(_)).Times(1).WillRepeatedly(testing::Return(remoteClient));
	_remoteControlClient->start();

	// simulate that someone on the RC Client wrote in the local console
	feedClient(_client, TEST_CONSOLE_ENTRY);

	// Check that the writer was called
	ASSERT_TRUE(checkClientWriterWasCalled(remoteClient, TEST_CONSOLE_ENTRY));
}

TEST_F(ConnectedModuleTests, test_RemoteControlClient_writesToConsole_When_clientReceivesMessage)
{
	auto connectionManagerMock = std::make_shared<ConnectionManagerMock>();
	makeRemoteControlClient(connectionManagerMock);
	auto remoteClient = std::make_shared<ClientMock>(_configuration);
	EXPECT_CALL(*connectionManagerMock, createClient(_)).Times(1).WillRepeatedly(testing::Return(remoteClient));
	_remoteControlClient->start();

	// simulate that the server replied (remoteClient is the server's handle)
	feedClient(remoteClient, TEST_CONSOLE_ENTRY);

	// Check that the console's writer was called
	ASSERT_TRUE(checkClientWriterWasCalled(_client, TEST_CONSOLE_ENTRY));
}

// RemoteAccessServer

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_doesNotStart_When_connectionMangerDoesNotCreate)
{
	std::vector<ghost::ConnectionConfiguration> confs({_configuration});
	_remoteAccessServer = std::make_shared<ghost::internal::RemoteAccessServer>(
	    confs, ghost::ConnectionManager::create(), _interpreter, ghost::StdoutLogger::create());

	ASSERT_FALSE(_remoteAccessServer->start());
}

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_doesNotStart_When_serverDoesNotStart)
{
	auto connectionManager = ghost::ConnectionManager::create();
	connectionManager->getConnectionFactory()->addServerRule<NotRunningServerMock>(_configuration);
	std::vector<ghost::ConnectionConfiguration> confs({_configuration});
	_remoteAccessServer = std::make_shared<ghost::internal::RemoteAccessServer>(
	    confs, connectionManager, _interpreter, ghost::StdoutLogger::create());

	ASSERT_FALSE(_remoteAccessServer->start());
}

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_starts_When_oneConfigurationIsSet)
{
	makeRemoteAccessServer(1);
	ASSERT_TRUE(_remoteAccessServer->start());
}

TEST_F(ConnectedModuleTests, test_RemoteAccessServer_starts_When_twoConfigurationsAreSet)
{
	makeRemoteAccessServer(2);
	ASSERT_TRUE(_remoteAccessServer->start());
}
