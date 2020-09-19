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

#ifndef GHOST_CONNECTIONGRPC_INTERNAL_CONNECTIONFACTORYRULEGRPC_HPP
#define GHOST_CONNECTIONGRPC_INTERNAL_CONNECTIONFACTORYRULEGRPC_HPP

#include <ghost/connection/ConnectionFactoryRule.hpp>
#include <ghost/module/ThreadPool.hpp>

namespace ghost
{
namespace internal
{
/**
 *	Rule for the ghost::ConnectionFactory that creates the gRPC-based connections.
 */
template <typename ConnectionType>
class ConnectionFactoryRuleGRPC : public ghost::ConnectionFactoryRule
{
public:
	ConnectionFactoryRuleGRPC(const ghost::ConnectionConfiguration& minimumConfiguration,
				  const std::shared_ptr<ghost::ThreadPool>& threadPool)
	    : ConnectionFactoryRule(minimumConfiguration), _threadPool(threadPool)
	{
	}

	std::shared_ptr<ghost::Connection> create(const ghost::ConnectionConfiguration& config) const override
	{
		return std::make_shared<ConnectionType>(config, _threadPool);
	}

private:
	std::shared_ptr<ghost::ThreadPool> _threadPool;
};

} // namespace internal
} // namespace ghost

#endif // GHOST_CONNECTIONGRPC_INTERNAL_CONNECTIONFACTORYRULEGRPC_HPP
