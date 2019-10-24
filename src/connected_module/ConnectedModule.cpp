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
				 const std::unique_ptr<ghost::ConnectionConfiguration>& remoteControlConfiguration)
    : _connectionManager(connectionManager)
    , _remoteAccessConfigurations(remoteAccessConfigurations)
{
	if (remoteControlConfiguration)
	{
		_remoteConfiguration = std::make_unique<ghost::ConnectionConfiguration>(*remoteControlConfiguration);
	}
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
	bool remoteAccessInitialized = initializeRemoteAccess();
	// Initialize the controlled remote module
	bool remoteControlInitialized = initializeRemoteControl();

	return remoteAccessInitialized && remoteControlInitialized;
}

void ConnectedModule::stop()
{
	// Stop the remote for the controlled remote module
	if (_remoteControl) _remoteControl->stop();

	// Stop the servers that handle remote controllers.
	if (_remoteAccess) _remoteAccess->stop();

	// Destroys the connection manager, this closes all open connections.
	_connectionManager.reset();
}

std::string ConnectedModule::getName() const
{
	return ghost::ConnectedModule::NAME;
}

bool ConnectedModule::initializeRemoteAccess()
{
	_remoteAccess = std::make_unique<RemoteAccessServer>(_remoteAccessConfigurations, _connectionManager,
							     getModule()->getInterpreter());

	// Start the remote
	return _remoteAccess->start();
}

bool ConnectedModule::initializeRemoteControl()
{
	// The user didn't configure a remote module
	if (!_remoteConfiguration) return true;

	_remoteControl = std::make_unique<RemoteControlClient>(
	    *_remoteConfiguration, _connectionManager, getModule()->getInterpreter(), getModule()->getConsole());

	// Start the remote
	return _remoteControl->start();
}
