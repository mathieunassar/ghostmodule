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

#include "RemoteControlClient.hpp"

using namespace ghost::internal;

const std::string RemoteControlClient::PREFIX_LOCAL_EXECUTION = "local:";

RemoteControlClient::RemoteControlClient(const ghost::ConnectionConfiguration& configuration,
					 const std::shared_ptr<ghost::ConnectionManager>& connectionManager,
					 const std::shared_ptr<ghost::CommandLineInterpreter>& commandLineInterpreter,
					 const std::shared_ptr<ghost::Console>& console,
					 const std::shared_ptr<ghost::Logger>& logger)
    : _configuration(configuration)
    , _connectionManager(connectionManager)
    , _interpreter(commandLineInterpreter)
    , _console(console)
    , _logger(logger)
{
}

bool RemoteControlClient::start()
{
	// Create the client corresponding to the configured remote
	_remote = _connectionManager->createClient(_configuration);
	if (!_remote)
	{
		GHOST_ERROR(_logger) << "Failed to register a client for the remote control.";
		stop();
		return false;
	}
	_remoteWriter = _remote->getWriter<google::protobuf::StringValue>();

	// Get the console from the parent module, fail if not configured
	if (!_console)
	{
		GHOST_ERROR(_logger)
		    << "No console was configured with this module, although it is configured to be a remote control.";
		stop();
		return false;
	}

	// Configure the console command callback to send messages to the remote
	_console->setCommandCallback(
	    std::bind(&RemoteControlClient::consoleCommandCallback, this, std::placeholders::_1));

	// Configure a message handler from the remote to write in the console
	auto messageHandler = _remote->addMessageHandler();
	messageHandler->addHandler<google::protobuf::StringValue>(
	    std::bind(&RemoteControlClient::remoteMessageCallback, this, std::placeholders::_1));

	// Start the remote
	bool startResult = _remote->start();
	if (!startResult)
	{
		GHOST_ERROR(_logger) << "Failed to start the remote control client.";
		stop();
	}
	return startResult;
}

void RemoteControlClient::stop()
{
	if (_remote) _remote->stop();
	_remote.reset();
	_remoteWriter.reset();
}

void RemoteControlClient::consoleCommandCallback(const std::string& command)
{
	// if the command starts with the substring PREFIX_LOCAL_EXECUTION, execute it locally.
	if (command.find(PREFIX_LOCAL_EXECUTION) == 0)
	{
		std::string realCommand = command.substr(PREFIX_LOCAL_EXECUTION.size());
		ghost::CommandExecutionContext context(ghost::Session::createLocal());
		context.setConsole(_console);
		_interpreter->execute(realCommand, context);
	}
	else
	{
		auto msg = google::protobuf::StringValue::default_instance();
		msg.set_value(command);
		_remoteWriter->write(msg);
	}
}

void RemoteControlClient::remoteMessageCallback(const google::protobuf::StringValue& message)
{
	_console->write(message.value());
}
