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

#include "../include/ghost/connection/NetworkConnectionConfiguration.hpp"

using namespace ghost;

namespace ghost
{
	namespace internal
	{
		static std::string NETWORKCONNECTIONCONFIGURATION_SERVERIP = "NETWORKCONNECTIONCONFIGURATION_SERVERIP";
		static std::string NETWORKCONNECTIONCONFIGURATION_SERVERPORT = "NETWORKCONNECTIONCONFIGURATION_SERVERPORT";
	}
}

NetworkConnectionConfiguration::NetworkConnectionConfiguration(const std::string& name)
	: ConnectionConfiguration(name)
{
	addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERIP, "");
	addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERPORT, "");
}

// getters of connection configuration parameters
std::string NetworkConnectionConfiguration::getServerIpAddress() const
{
	std::string res;
	getAttribute<std::string>(internal::NETWORKCONNECTIONCONFIGURATION_SERVERIP, res, ""); // if the field was removed, returns ""
	return res;
}

int NetworkConnectionConfiguration::getServerPortNumber() const
{
	int res;
	getAttribute<int>(internal::NETWORKCONNECTIONCONFIGURATION_SERVERPORT, res, -1); // if the field was removed, returns -1
	return res;
}

// setters of connection configuration parameters
void NetworkConnectionConfiguration::setServerIpAddress(const std::string& ip)
{
	addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERIP, ip, true); // checks if the attribute is there as well
}

void NetworkConnectionConfiguration::setServerPortNumber(int port)
{
	addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERPORT, port, true); // checks if the attribute is there as well
}

NetworkConnectionConfiguration NetworkConnectionConfiguration::initializeFrom(const ConnectionConfiguration& from)
{
	NetworkConnectionConfiguration newconfig(from.getConfigurationName());
	from.copy(newconfig);
	return newconfig;
}
