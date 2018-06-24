#include "../include/NetworkConnectionConfiguration.hpp"

using namespace ghost;

namespace ghost::internal
{
	static std::string NETWORKCONNECTIONCONFIGURATION_SERVERIP = "NETWORKCONNECTIONCONFIGURATION_SERVERIP";
	static std::string NETWORKCONNECTIONCONFIGURATION_SERVERPORT = "NETWORKCONNECTIONCONFIGURATION_SERVERPORT";
}

NetworkConnectionConfiguration::NetworkConnectionConfiguration(const std::string& name)
	: ConnectionConfiguration(name)
{
	addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERIP, "");
	addAttribute(internal::NETWORKCONNECTIONCONFIGURATION_SERVERPORT, (int)-1);
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
