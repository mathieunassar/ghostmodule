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

#include "ConnectionExtensionBuilder.hpp"

#include "ConnectionExtension.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::ConnectionExtensionBuilder> ghost::ConnectionExtensionBuilder::create()
{
	return std::make_shared<ghost::internal::ConnectionExtensionBuilder>();
}

ConnectionExtensionBuilder::ConnectionExtensionBuilder()
    : _connectionManager(ghost::ConnectionManager::create())
{
}

// From ghost::ConnectivityComponentBuilder
std::shared_ptr<ghost::ConnectionManager> ConnectionExtensionBuilder::configureConnectionManager()
{
	return _connectionManager;
}

ConnectionExtensionBuilder& ConnectionExtensionBuilder::addRemoteAccess(
    const ghost::ConnectionConfiguration& configuration)
{
	_remoteAccessConfigurations.push_back(configuration);
	return *this;
}

ConnectionExtensionBuilder& ConnectionExtensionBuilder::setRemoteControl(
    const ghost::ConnectionConfiguration& configuration)
{
	_remoteControlConfiguration = std::make_unique<ghost::ConnectionConfiguration>(configuration);
	return *this;
}

// From ghost::ModuleComponentBuilder
std::shared_ptr<ghost::ModuleExtension> ConnectionExtensionBuilder::build()
{
	return std::make_shared<ghost::internal::ConnectionExtension>(_connectionManager, _remoteAccessConfigurations,
								  _remoteControlConfiguration);
}
