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

#ifndef GHOST_INTERNAL_CONNECTED_MODULE_COMPONENT_BUILDER_HPP
#define GHOST_INTERNAL_CONNECTED_MODULE_COMPONENT_BUILDER_HPP

#include <ghost/connected_module/ConnectedModuleComponentBuilder.hpp>
#include <ghost/module/ModuleComponentBuilder.hpp>
#include <optional>

namespace ghost
{
namespace internal
{
class ConnectedModuleComponentBuilder : public ghost::ConnectedModuleComponentBuilder
{
public:
	ConnectedModuleComponentBuilder();

	// From ghost::ConnectivityComponentBuilder
	std::shared_ptr<ghost::ConnectionManager> configureConnectionManager() override;
	ConnectedModuleComponentBuilder& addRemoteAccess(const ghost::ConnectionConfiguration& configuration) override;
	ConnectedModuleComponentBuilder& setRemoteControl(const ghost::ConnectionConfiguration& configuration) override;

	// From ghost::ModuleComponentBuilder
	std::shared_ptr<ghost::ModuleComponent> build() override;

private:
	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
	std::vector<ghost::ConnectionConfiguration> _remoteAccessConfigurations;
	std::optional<ghost::ConnectionConfiguration> _remoteControlConfiguration;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_CONNECTED_MODULE_COMPONENT_BUILDER_HPP
