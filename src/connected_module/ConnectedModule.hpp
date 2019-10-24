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

#ifndef GHOST_INTERNAL_CONNECTEDMODULE_HPP
#define GHOST_INTERNAL_CONNECTEDMODULE_HPP

#include <ghost/connected_module/ConnectedModule.hpp>
#include <ghost/module/ModuleComponent.hpp>
#include <memory>
#include "RemoteAccessServer.hpp"
#include "RemoteControlClient.hpp"

namespace ghost
{
namespace internal
{
class ConnectedModule : public ghost::ConnectedModule, public ghost::ModuleComponent
{
public:
	ConnectedModule(const std::shared_ptr<ghost::ConnectionManager>& connectionManager,
			const std::vector<ghost::ConnectionConfiguration>& remoteAccessConfigurations,
			const std::unique_ptr<ghost::ConnectionConfiguration>& remoteControlConfiguration);

	// From ghost::Connectivity
	std::shared_ptr<ghost::ConnectionManager> getConnectionManager() const override;

	// From ghost::ModuleComponent
	bool start() override;
	void stop() override;
	std::string getName() const override;

private:
	bool initializeRemoteAccess();
	bool initializeRemoteControl();

	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
	// Remote Access
	std::vector<ghost::ConnectionConfiguration> _remoteAccessConfigurations;
	std::unique_ptr<RemoteAccessServer> _remoteAccess;
	// Remote Control
	std::unique_ptr<ghost::ConnectionConfiguration> _remoteConfiguration;
	std::unique_ptr<RemoteControlClient> _remoteControl;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_CONNECTEDMODULE_HPP
