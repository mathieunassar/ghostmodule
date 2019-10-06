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

#include "ConnectivityComponentBuilder.hpp"
#include "Connectivity.hpp"

using namespace ghost::internal;

std::unique_ptr<ghost::ConnectivityComponentBuilder> ghost::ConnectivityComponentBuilder::create()
{
	return std::make_unique<ghost::internal::ConnectivityComponentBuilder>();
}

ConnectivityComponentBuilder::ConnectivityComponentBuilder()
    : _connectionManager(ghost::ConnectionManager::create()), _remoteController(false)
{
}

// From ghost::ConnectivityComponentBuilder
std::shared_ptr<ghost::ConnectionFactory> ConnectivityComponentBuilder::configureConnectionFactory()
{
	return _connectionManager->getConnectionFactory();
}

ConnectivityComponentBuilder& ConnectivityComponentBuilder::addRemoteAccess(
    const ghost::ConnectionConfiguration& configuration)
{
	_remoteAccessConfigurations.push_back(configuration);
	return *this;
}

ConnectivityComponentBuilder& ConnectivityComponentBuilder::setRemoteControl(
    const ghost::ConnectionConfiguration& configuration)
{
	_remoteController = true;
	_remoteControlConfiguration = configuration;
	return *this;
}

// From ghost::ModuleComponentBuilder
std::shared_ptr<ghost::ModuleComponent> ConnectivityComponentBuilder::build()
{
	return std::make_shared<ghost::internal::Connectivity>(_connectionManager);
}
