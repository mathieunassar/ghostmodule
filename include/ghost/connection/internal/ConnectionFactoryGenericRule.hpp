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

#ifndef GHOST_INTERNAL_CONNECTIONFACTORYGENERICRULE_HPP
#define GHOST_INTERNAL_CONNECTIONFACTORYGENERICRULE_HPP

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/ConnectionConfiguration.hpp>
#include <ghost/connection/ConnectionFactoryRule.hpp>
#include <memory>

namespace ghost
{
namespace internal
{
/* Generic class */
template <typename ConnectionType>
class ConnectionFactoryGenericRule : public ghost::ConnectionFactoryRule
{
public:
	ConnectionFactoryGenericRule(const ghost::ConnectionConfiguration& minimumConfiguration)
	    : ConnectionFactoryRule(minimumConfiguration)
	{
	}

	std::shared_ptr<ghost::Connection> create(const ghost::ConnectionConfiguration& config) const override
	{
		return std::make_shared<ConnectionType>(config);
	}
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_CONNECTIONFACTORYGENERICRULE_HPP
