#ifndef GHOST_NETWORKCONNECTIONCONFIGURATION_HPP
#define GHOST_NETWORKCONNECTIONCONFIGURATION_HPP

#include <ghost/connection/ConnectionConfiguration.hpp>

namespace ghost
{
	/**
	 * @brief Extended connection configuration for network connections.
	 * 
	 * This connection configuration contains additional required parameters
	 * for network connections:
	 * - The IP address of the target connection
	 * - the port number to reach.
	 */
	class NetworkConnectionConfiguration : public ConnectionConfiguration
	{
	public:
		/**
		 * @brief Constructs a new NetworkConnectionConfiguration object with
		 * default parameters, i.e. any IP address and any remote port number.
		 * 
		 * @param name the name of the configuration
		 */
		NetworkConnectionConfiguration(const std::string& name = "");
		virtual ~NetworkConnectionConfiguration() {}
		 
		/**
		 * @brief Accessor for the IP address of the target connection
		 * 
		 * @return the requested IP address
		 */
		std::string getServerIpAddress() const;
		/**
		 * @brief Accessor for the port number of the target connection
		 * 
		 * @return the requested port number
		 */
		int getServerPortNumber() const;

		/**
		 * @brief Set the IP address of the target connection
		 * 
		 * @param ip the new IP address to set
		 */
		void setServerIpAddress(const std::string& ip);
		/**
		 * @brief Set the server port number of the target connection
		 * 
		 * @param port the new port number to set
		 */
		void setServerPortNumber(int port);

		/**
		 * @brief Creates a network connection configuration from a connection
		 * configuration
		 * 
		 * @param from source connection configuration
		 * 
		 * @return NetworkConnectionConfiguration the created network connection configuration
		 */
		static NetworkConnectionConfiguration initializeFrom(const ConnectionConfiguration& from);
	};
}

#endif //GHOST_CONNECTIONCONFIGURATION_HPP
