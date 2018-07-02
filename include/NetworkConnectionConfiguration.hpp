#ifndef GHOST_NETWORKCONNECTIONCONFIGURATION_HPP
#define GHOST_NETWORKCONNECTIONCONFIGURATION_HPP

#include "ConnectionConfiguration.hpp"

namespace ghost
{
	class NetworkConnectionConfiguration : public ConnectionConfiguration
	{
	public:
		NetworkConnectionConfiguration(const std::string& name = "");
		virtual ~NetworkConnectionConfiguration() {}
		 
		// getters of connection configuration parameters
		std::string getServerIpAddress() const;
		int getServerPortNumber() const;

		// setters of connection configuration parameters
		void setServerIpAddress(const std::string& ip);
		void setServerPortNumber(int port);
	};
}

#endif //GHOST_CONNECTIONCONFIGURATION_HPP
