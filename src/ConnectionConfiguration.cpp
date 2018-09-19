#include "../include/ConnectionConfiguration.hpp"

using namespace ghost;

namespace ghost::internal
{
	static std::string CONNECTIONCONFIGURATION_ID = "CONNECTIONCONFIGURATION_ID";
	static std::string CONNECTIONCONFIGURATION_THREADPOOLSIZE = "CONNECTIONCONFIGURATION_THREADPOOLSIZE";
	static std::string CONNECTIONCONFIGURATION_BLOCKING = "CONNECTIONCONFIGURATION_BLOCKING";
}

ConnectionConfiguration::ConnectionConfiguration(const std::string& name)
	: Configuration(name)
{
	addAttribute(internal::CONNECTIONCONFIGURATION_ID, (int)-1);
	addAttribute(internal::CONNECTIONCONFIGURATION_THREADPOOLSIZE, (int)2);
	addAttribute(internal::CONNECTIONCONFIGURATION_BLOCKING, true);
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
