/*
 * Copyright 2020 Mathieu Nassar
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

#ifndef GHOST_CONNECTIONEXTENSION_HPP
#define GHOST_CONNECTIONEXTENSION_HPP

#include <ghost/connection/ConnectionManager.hpp>
#include <string>

namespace ghost
{
/**
 *	The ghost::ConnectionExtension extension integrates the features of the
 *	ghost_connection library into a ghost::Module.
 *
 *	In order to use this class, use the ghost::ConnectionExtensionBuilder to configure
 *	this component and pass the builder to an instance of ghost::ModuleBuilder.
 *	This component provides a ghost::ConnectionManger that can be configured and used to
 *	create ghost::Connection objects.
 *
 *	During the build phase or the initialization phase, it is possible to configure a remote
 *	control, i.e. to use this module as a proxy for another module. The controlled module must
 *	have integrated this component as well and configured the builder with a remote access.
 */
class ConnectionExtension
{
public:
	/// Name of this component. Used by ghost::Module to identify this component.
	static const std::string getExtensionName();

	virtual ~ConnectionExtension() = default;

	/**
	 *	@return the instance of ghost::ConnectionManager used by the component. This instance
	 *		may be used to create ghost::Connections to communicate with other modules.
	 */
	virtual std::shared_ptr<ghost::ConnectionManager> getConnectionManager() const = 0;

	/**
	 *	During the module's initialization phase, this method configures the module to be a remote
	 *	control for the module listening to the connection referenced by the "configuration" parameter.
	 *	Once this method is called, user input from the configured ghost::Console is forwarded to the
	 *	controlled module.
	 *
	 *	The intialization of this component fails if no ghost::Console was configured in the
	 *	ghost::ModuleBuilder. The intialization of this component also fails if the module to control is not
	 *	listening to connection referenced by the "configuration" parameter.
	 *
	 *	This method has no effect if called after the module's initialization phase.
	 *	@param configuration	connection information pointing to the module to control.
	 */
	virtual void setRemoteControl(const ghost::ConnectionConfiguration& configuration) = 0;

	/**
	 *	During the module's initialization phase, this method configures the module to be remotely
	 *	controllable with a server listening to the connection referenced by the "configuration" parameter.

	 *	This method has no effect if called after the module's initialization phase.
	 *	@param configuration	configuration of a server listening for remote controlling clients.
	 */
	virtual void addRemoteAccess(const ghost::ConnectionConfiguration& configuration) = 0;
};
} // namespace ghost

#endif // GHOST_CONNECTIONEXTENSION_HPP
