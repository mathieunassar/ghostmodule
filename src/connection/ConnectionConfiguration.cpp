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

#include <ghost/connection/ConnectionConfiguration.hpp>

using namespace ghost;

namespace ghost
{
	namespace internal
	{
		static std::string CONNECTIONCONFIGURATION_ID = "CONNECTIONCONFIGURATION_ID";
		static std::string CONNECTIONCONFIGURATION_THREADPOOLSIZE = "CONNECTIONCONFIGURATION_THREADPOOLSIZE";
		static std::string CONNECTIONCONFIGURATION_BLOCKING = "CONNECTIONCONFIGURATION_BLOCKING";
	}
}

ConnectionConfiguration::ConnectionConfiguration(const std::string& name)
{
	_configuration = ghost::Configuration::create(name);

	ghost::ConfigurationValue defaultId;
	defaultId.write((int)-1);
	_configuration->addAttribute(internal::CONNECTIONCONFIGURATION_ID, defaultId);

	ghost::ConfigurationValue defaultThreadPoolSize;
	defaultThreadPoolSize.write((size_t)2);
	_configuration->addAttribute(internal::CONNECTIONCONFIGURATION_THREADPOOLSIZE, defaultThreadPoolSize);

	ghost::ConfigurationValue defaultBlocking;
	defaultBlocking.write(true);
	_configuration->addAttribute(internal::CONNECTIONCONFIGURATION_BLOCKING, defaultBlocking);
}

int ConnectionConfiguration::getConnectionId() const
{
	int res;
	ConfigurationValue value;
	ConfigurationValue default;
	default.write<int>(-1);
	
	_configuration->getAttribute(internal::CONNECTIONCONFIGURATION_ID, value, default); // if the field was removed, returns -1
	value.read<int>(res);
	
	return res;
}

size_t ConnectionConfiguration::getThreadPoolSize() const
{
	size_t res;
	ConfigurationValue value;
	ConfigurationValue default;
	default.write<size_t>(2);

	_configuration->getAttribute(internal::CONNECTIONCONFIGURATION_THREADPOOLSIZE, value, default); // if the field was removed, returns 2
	value.read<size_t>(res);

	return res;
}

bool ConnectionConfiguration::isOperationBlocking() const
{
	bool res;
	ConfigurationValue value;
	ConfigurationValue default;
	default.write<size_t>(true);

	_configuration->getAttribute(internal::CONNECTIONCONFIGURATION_BLOCKING, value, default); // if the field was removed, returns true
	value.read<bool>(res);

	return res;
}

// setters of connection configuration parameters
void ConnectionConfiguration::setConnectionId(int id)
{
	ghost::ConfigurationValue value;
	value.write<int>(id);
	_configuration->addAttribute(internal::CONNECTIONCONFIGURATION_ID, value, true); // checks if the attribute is there as well
}

void ConnectionConfiguration::setThreadPoolSize(size_t size)
{
	ghost::ConfigurationValue value;
	value.write<size_t>(size);
	_configuration->addAttribute(internal::CONNECTIONCONFIGURATION_THREADPOOLSIZE, value, true); // checks if the attribute is there as well
}

void ConnectionConfiguration::setOperationBlocking(bool blocking)
{
	ghost::ConfigurationValue value;
	value.write<bool>(blocking);
	_configuration->addAttribute(internal::CONNECTIONCONFIGURATION_BLOCKING, value, true); // checks if the attribute is there as well
}

std::shared_ptr<ghost::Configuration> ConnectionConfiguration::getConfiguration() const
{
	return _configuration;
}
