#ifndef GHOST_CONNECTIONCONFIGURATION_HPP
#define GHOST_CONNECTIONCONFIGURATION_HPP

#include <ghost/connection/Configuration.hpp>

namespace ghost
{
	/**
	 * @brief A Configuration with specific entries for Connection objects.
	 */
	class ConnectionConfiguration : public Configuration
	{
	public:
		/**
		 * @brief Construct a new ConnectionConfiguration object
		 * 
		 * @param name name of the configuration.
		 */
		ConnectionConfiguration(const std::string& name = "");
		virtual ~ConnectionConfiguration() {}

		/**
		 * @return the ID of the connection. 
		 */
		int getConnectionId() const;
		
		/**
		 * @return the desized size of the thread pool used by a connection.
		 */
		size_t getThreadPoolSize() const;
		
		/**
		 * @return true if i/o operations should be blocking
		 * @return false uf i/o operations should be non-blocking
		 */
		bool isOperationBlocking() const;

		/**
		 * @param id the ID of the connection
		 */
		void setConnectionId(int id);

		/**
		 * @param size the size of the thread pool used by a connection.
		 */
		void setThreadPoolSize(size_t size);

		/**
		 * @param value true if the i/o operations should be blocking, false otherwise.
		 */
		void setOperationBlocking(bool value);
	};
}

#endif //GHOST_CONNECTIONCONFIGURATION_HPP
