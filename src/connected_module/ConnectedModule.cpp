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

#include "ConnectedModule.hpp"
#include <ghost/module/Module.hpp>

using namespace ghost::internal;

const std::string ghost::ConnectedModule::NAME = "ConnectedModule";

ConnectedModule::ConnectedModule(const std::shared_ptr<ghost::ConnectionManager>& connectionManager,
				 const std::vector<ghost::ConnectionConfiguration>& remoteAccessConfigurations,
				 const std::optional<ghost::ConnectionConfiguration>& remoteControlConfiguration)
    : _connectionManager(connectionManager)
    , _remoteAccessConfigurations(remoteAccessConfigurations)
    , _remoteConfiguration(remoteControlConfiguration)
{
}

std::shared_ptr<ghost::ConnectionManager> ConnectedModule::getConnectionManager() const
{
	return _connectionManager;
}

// From ghost::ModuleComponent
bool ConnectedModule::start()
{
	// Sanity check to not crash if no parent module was set.
	if (!getModule()) return false;

	// Initialize the remote controllers
	bool remoteAccessInitialized = initializeRemoteAccess(_remoteAccessConfigurations);
	// Initialize the controlled remote module
	bool remoteControlInitialized = initializeRemoteControl();

	return remoteAccessInitialized && remoteControlInitialized;
}

void ConnectedModule::stop()
{
	// Stop the remote for the controlled remote module
	if (_remote) _remote->stop();
	_remote.reset();
	_remoteWriter.reset();

	// Stop the servers that handle remote controllers.
	for (const auto& server : _remoteControllersServers) server->stop();
	_remoteControllersServers.clear();
	_remoteControllersHandler.reset();

	// Destroys the connection manager, this closes all open connections.
	_connectionManager.reset();
}

std::string ConnectedModule::getName() const
{
	return ghost::ConnectedModule::NAME;
}

bool ConnectedModule::initializeRemoteAccess(
    const std::vector<ghost::ConnectionConfiguration>& remoteAccessConfigurations)
{
	_remoteControllersHandler = std::make_shared<RemoteControllersHandler>(getModule()->getInterpreter());

	for (const auto& configuration : remoteAccessConfigurations)
	{
		// Create the remote access server for this configuration and check that it worked
		auto server = _connectionManager->createServer(configuration);
		if (!server) return false;

		// Sets the remote controller handler to process clients of this server
		server->setClientHandler(_remoteControllersHandler);

		// Start the server, it may now receive clients
		if (!server->start()) return false;

		// Add the server to the list of remote controller servers.
		_remoteControllersServers.push_back(server);
	}

	return true;
}

bool ConnectedModule::initializeRemoteControl()
{
	// The user didn't configure a remote module
	if (!_remoteConfiguration) return true;

	// Create the client corresponding to the configured remote
	_remote = _connectionManager->createClient(*_remoteConfiguration);
	if (!_remote) return false;
	_remoteWriter = _remote->getWriter<google::protobuf::StringValue>();

	// Get the console from the parent module, fail if not configured
	auto console = getModule()->getConsole();
	if (!console) return false;

	// Configure the console command callback to send messages to the remote
	console->setCommandCallback([&](const std::string& command) {
		auto msg = google::protobuf::StringValue::default_instance();
		msg.set_value(command);
		_remoteWriter->write(msg);
	});

	// Configure a message handler from the remote to write in the console
	auto messageHandler = _remote->addMessageHandler();
	messageHandler->addHandler<google::protobuf::StringValue>(
	    [&](const google::protobuf::StringValue& msg) { console->write(msg.value()); });

	// Start the remote
	return _remote->start();
}
