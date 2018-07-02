#ifndef GHOST_CONNECTIONCONFIGURATION_HPP
#define GHOST_CONNECTIONCONFIGURATION_HPP

#include "Configuration.hpp"

namespace ghost
{
	class ConnectionConfiguration : public Configuration
	{
	public:
		ConnectionConfiguration(const std::string& name = "");
		virtual ~ConnectionConfiguration() {}
		 
		// getters of connection configuration parameters
		int getConnectionId() const;
		int getThreadPoolSize() const;
		bool isOperationBlocking() const;

		// setters of connection configuration parameters
		void setConnectionId(int id);
		void setThreadPoolSize(int size);
		void setOperationBlocking(bool value);
	};
}

#endif //GHOST_CONNECTIONCONFIGURATION_HPP
