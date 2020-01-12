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

#include <ghost/connection_grpc/ConnectionConfigurationGRPC.hpp>

using namespace ghost;

namespace ghost
{
namespace internal
{
static std::string CONNECTIONCONFIGURATIONGRPC_TECHNOLOGY = "CONNECTIONCONFIGURATIONGRPC_TECHNOLOGY";
}
} // namespace ghost

ConnectionConfigurationGRPC::ConnectionConfigurationGRPC(const std::string& name) : NetworkConnectionConfiguration(name)
{
	ghost::ConfigurationValue techonologyAttribute;
	_configuration->addAttribute(internal::CONNECTIONCONFIGURATIONGRPC_TECHNOLOGY, techonologyAttribute);
}

ConnectionConfigurationGRPC::ConnectionConfigurationGRPC(const std::string& ip, int port)
    : ConnectionConfigurationGRPC("")
{
	setServerIpAddress(ip);
	setServerPortNumber(port);
}
