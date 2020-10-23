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

#ifndef GHOST_CONNECTION_CONNECTIONFACTORYRULE_HPP
#define GHOST_CONNECTION_CONNECTIONFACTORYRULE_HPP

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/ConnectionConfiguration.hpp>
#include <memory>

namespace ghost
{
/**
 *	Base class for connection creation rules within the ghost::ConnectionFactory.
 *	The create method is used to generate the connection linked to this rule.
 */
class ConnectionFactoryRule
{
public:
	ConnectionFactoryRule(const ghost::ConnectionConfiguration& minimumConfiguration);
	virtual ~ConnectionFactoryRule() = default;

	/**
	 *	returns true if the candidate configuration fulfills the minimum configuration requirements
	 *	Note: empty values in the minimum configuration mean that any value is allowed for this parameter
	 */
	bool matches(const ghost::ConnectionConfiguration& candidate) const;
	/// creates an instance of the corresponding connection, with the given configuration in parameters.
	virtual std::shared_ptr<ghost::Connection> create(const ghost::ConnectionConfiguration& config) const = 0;

protected:
	ghost::ConnectionConfiguration _minimumConfiguration;
};

} // namespace ghost

#endif // GHOST_CONNETION_CONNECTIONFACTORYRULE_HPP
