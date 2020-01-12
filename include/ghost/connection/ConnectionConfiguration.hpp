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

#ifndef GHOST_CONNECTIONCONFIGURATION_HPP
#define GHOST_CONNECTIONCONFIGURATION_HPP

#include <ghost/connection/Configuration.hpp>
#include <memory>
#include <string>

namespace ghost
{
/**
 * @brief A Configuration with specific entries for Connection objects.
 */
class ConnectionConfiguration
{
public:
	ConnectionConfiguration(const std::string& name = "");
	virtual ~ConnectionConfiguration() = default;

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

	/**
	 *	@return the configuration used by this object.
	 */
	std::shared_ptr<ghost::Configuration> getConfiguration() const;

protected:
	std::shared_ptr<ghost::Configuration> _configuration;
};
} // namespace ghost

#endif // GHOST_CONNECTIONCONFIGURATION_HPP
