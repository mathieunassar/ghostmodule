#ifndef GHOST_CONNECTIONCONFIGURATION_HPP
#define GHOST_CONNECTIONCONFIGURATION_HPP

#include "Configuration.hpp"

namespace ghost
{
	class ConnectionConfiguration : public Configuration
	{
	public:
		ConnectionConfiguration(const std::string& name = "");
		 
		// getters of connection configuration parameters
		int getConnectionId() const;
		int getThreadPoolSize() const;

		// setters of connection configuration parameters
		void setConnectionId(int id);
		void setThreadPoolSize(int size);
	};
}

#endif //GHOST_CONNECTIONCONFIGURATION_HPP
