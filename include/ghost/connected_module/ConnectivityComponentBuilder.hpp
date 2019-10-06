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

#ifndef GHOST_CONNECTIVITY_COMPONENT_BUILDER_HPP
#define GHOST_CONNECTIVITY_COMPONENT_BUILDER_HPP

#include <ghost/connection/ConnectionConfiguration.hpp>
#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/module/ModuleComponent.hpp>
#include <string>

namespace ghost
{
/**
 *	Builder for the ghost::Connectivity component.
 *	The ghost::Connectivity component allows a module to be configured to control
 *	a distant server and to be controlled by a remote client.
 *	The ghost::Connectivity also provides a ghost::ConnectionManager which can be
 *	used to manage the module's ghost::Connection objects.
 *
 *	Add this ghost::ConnectivityComponentBuilder to an instance of ghost::ModuleBuilder
 *	in order to build the corresponding module.
 */
class ConnectivityComponentBuilder
{
public:
	static std::unique_ptr<ghost::ConnectivityComponentBuilder> create();

	virtual ~ConnectivityComponentBuilder() = default;
	/**
	 *	Gets the ghost::ConnectionFactory used by the module.
	 *	The factory is used to create connections, in particular in combination
	 *	with the module's settings set by "addRemoteAccess" and "setRemoteControl".
	 *	When using this builder, the ghost::ConnectionFactory must be configured
	 *	before calling "build" on the owning ghost::ModuleBuilder.
	 */
	virtual std::shared_ptr<ghost::ConnectionFactory> configureConnectionFactory() = 0;
	/**
	 *	Adds a configuration for a server accepting clients that can control this module.
	 *	@param configuration	connection configuration of the local server accepting clients.
	 *	@return an instance of this.
	 */
	virtual ConnectivityComponentBuilder& addRemoteAccess(const ghost::ConnectionConfiguration& configuration) = 0;
	/**
	 *	Set the configuration of the remote server to control.
	 *	This configured the module to forward user input to the remote server.
	 *	@param configuration	configuration of a remote server to control.
	 *	@return an instance of this.
	 */
	virtual ConnectivityComponentBuilder& setRemoteControl(const ghost::ConnectionConfiguration& configuration) = 0;
};
} // namespace ghost

#endif // GHOST_CONNECTIVITY_COMPONENT_BUILDER_HPP
