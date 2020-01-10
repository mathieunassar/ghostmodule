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

#include "ConnectedModuleComponentBuilder.hpp"

#include "ConnectedModule.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::ConnectedModuleComponentBuilder> ghost::ConnectedModuleComponentBuilder::create()
{
	return std::make_shared<ghost::internal::ConnectedModuleComponentBuilder>();
}

ConnectedModuleComponentBuilder::ConnectedModuleComponentBuilder()
    : _connectionManager(ghost::ConnectionManager::create())
{
}

// From ghost::ConnectivityComponentBuilder
std::shared_ptr<ghost::ConnectionManager> ConnectedModuleComponentBuilder::configureConnectionManager()
{
	return _connectionManager;
}

ConnectedModuleComponentBuilder& ConnectedModuleComponentBuilder::addRemoteAccess(
    const ghost::ConnectionConfiguration& configuration)
{
	_remoteAccessConfigurations.push_back(configuration);
	return *this;
}

ConnectedModuleComponentBuilder& ConnectedModuleComponentBuilder::setRemoteControl(
    const ghost::ConnectionConfiguration& configuration)
{
	_remoteControlConfiguration = std::make_unique<ghost::ConnectionConfiguration>(configuration);
	return *this;
}

// From ghost::ModuleComponentBuilder
std::shared_ptr<ghost::ModuleComponent> ConnectedModuleComponentBuilder::build()
{
	return std::make_shared<ghost::internal::ConnectedModule>(_connectionManager, _remoteAccessConfigurations,
								  _remoteControlConfiguration);
}
