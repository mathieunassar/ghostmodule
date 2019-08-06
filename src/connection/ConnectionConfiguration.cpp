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

#include "../include/ghost/connection/ConnectionConfiguration.hpp"

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
	: Configuration(name)
{
	addAttribute(internal::CONNECTIONCONFIGURATION_ID, "");
	addAttribute(internal::CONNECTIONCONFIGURATION_THREADPOOLSIZE, "");
	addAttribute(internal::CONNECTIONCONFIGURATION_BLOCKING, "");
}

int ConnectionConfiguration::getConnectionId() const
{
	int res;
	getAttribute<int>(internal::CONNECTIONCONFIGURATION_ID, res, -1); // if the field was removed, returns -1
	return res;
}

size_t ConnectionConfiguration::getThreadPoolSize() const
{
	size_t res;
	getAttribute<size_t>(internal::CONNECTIONCONFIGURATION_THREADPOOLSIZE, res, 2); // if the field was removed, returns -1
	return res;
}

bool ConnectionConfiguration::isOperationBlocking() const
{
	bool res;
	getAttribute<bool>(internal::CONNECTIONCONFIGURATION_BLOCKING, res, true); // if the field was removed, returns -1
	return res;
}

// setters of connection configuration parameters
void ConnectionConfiguration::setConnectionId(int id)
{
	addAttribute(internal::CONNECTIONCONFIGURATION_ID, id, true); // checks if the attribute is there as well
}

void ConnectionConfiguration::setThreadPoolSize(size_t size)
{
	addAttribute(internal::CONNECTIONCONFIGURATION_THREADPOOLSIZE, size, true); // checks if the attribute is there as well
}

void ConnectionConfiguration::setOperationBlocking(bool value)
{
	addAttribute(internal::CONNECTIONCONFIGURATION_BLOCKING, value, true); // checks if the attribute is there as well
}
