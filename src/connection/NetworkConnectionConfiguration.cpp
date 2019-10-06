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

#include <ghost/connection/NetworkConnectionConfiguration.hpp>

using namespace ghost;

namespace ghost
{
namespace internal
{
static std::string NETWORKCONNECTIONCONFIGURATION_SERVERIP = "NETWORKCONNECTIONCONFIGURATION_SERVERIP";
static std::string NETWORKCONNECTIONCONFIGURATION_SERVERPORT = "NETWORKCONNECTIONCONFIGURATION_SERVERPORT";
} // namespace internal
} // namespace ghost

NetworkConnectionConfiguration::NetworkConnectionConfiguration(const std::string& name) : ConnectionConfiguration(name)
{
	// Default values are provided in the getters. A default construction creates empty values (For minimal
	// configurations)
	ghost::ConfigurationValue defaultIp;
	_configuration->addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERIP, defaultIp);

	ghost::ConfigurationValue defaultPort;
	_configuration->addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERPORT, defaultPort);
}

// getters of connection configuration parameters
std::string NetworkConnectionConfiguration::getServerIpAddress() const
{
	std::string res;
	ConfigurationValue value;
	ConfigurationValue defaultValue("127.0.0.1");

	_configuration->getAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERIP, value,
				     defaultValue); // if the field was removed, returns ""
	if (!value.read<std::string>(res)) defaultValue.read<std::string>(res);

	return res;
}

int NetworkConnectionConfiguration::getServerPortNumber() const
{
	int res = -1;
	ConfigurationValue value;
	ConfigurationValue defaultValue;
	defaultValue.write<int>(-1);

	_configuration->getAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERPORT, value,
				     defaultValue); // if the field was removed, returns -1
	value.read<int>(res);

	return res;
}

// setters of connection configuration parameters
void NetworkConnectionConfiguration::setServerIpAddress(const std::string& ip)
{
	ghost::ConfigurationValue value;
	value.write<std::string>(ip);
	_configuration->addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERIP, value,
				     true); // checks if the attribute is there as well
}

void NetworkConnectionConfiguration::setServerPortNumber(int port)
{
	ghost::ConfigurationValue value;
	value.write<int>(port);
	_configuration->addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERPORT, value,
				     true); // checks if the attribute is there as well
}

NetworkConnectionConfiguration NetworkConnectionConfiguration::initializeFrom(const ConnectionConfiguration& from)
{
	NetworkConnectionConfiguration newconfig(from.getConfiguration()->getConfigurationName());
	from.getConfiguration()->copy(*newconfig.getConfiguration());
	return newconfig;
}
