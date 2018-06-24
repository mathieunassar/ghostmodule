#include "../include/ConnectionConfiguration.hpp"

using namespace ghost;

namespace ghost::internal
{
	static std::string CONNECTIONCONFIGURATION_ID = "CONNECTIONCONFIGURATION_ID";
	static std::string CONNECTIONCONFIGURATION_THREADPOOLSIZE = "CONNECTIONCONFIGURATION_THREADPOOLSIZE";
}

ConnectionConfiguration::ConnectionConfiguration(const std::string& name)
	: Configuration(name)
{
	addAttribute(internal::CONNECTIONCONFIGURATION_ID, (int)-1);
}

int ConnectionConfiguration::getConnectionId() const
{
	int res;
	getAttribute<int>(internal::CONNECTIONCONFIGURATION_ID, res, -1); // if the field was removed, returns -1
	return res;
}

int ConnectionConfiguration::getThreadPoolSize() const
{
	int res;
	getAttribute<int>(internal::CONNECTIONCONFIGURATION_THREADPOOLSIZE, res, 2); // if the field was removed, returns -1
	return res;
}

// setters of connection configuration parameters
void ConnectionConfiguration::setConnectionId(int id)
{
	addAttribute(internal::CONNECTIONCONFIGURATION_ID, id, true); // checks if the attribute is there as well
}

void ConnectionConfiguration::setThreadPoolSize(int size)
{
	addAttribute(internal::CONNECTIONCONFIGURATION_THREADPOOLSIZE, size, true); // checks if the attribute is there as well
}