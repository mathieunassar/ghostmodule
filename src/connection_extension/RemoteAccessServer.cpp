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

#include "RemoteAccessServer.hpp"

using namespace ghost::internal;

RemoteAccessServer::RemoteAccessServer(const std::vector<ghost::ConnectionConfiguration>& configurations,
				       const std::shared_ptr<ghost::ConnectionManager>& connectionManager,
				       const std::shared_ptr<ghost::CommandLineInterpreter>& commandLineInterpreter,
				       const std::shared_ptr<ghost::ThreadPool>& threadPool,
				       const std::shared_ptr<ghost::Logger>& logger)
    : _connectionManager(connectionManager)
    , _remoteAccessConfigurations(configurations)
    , _interpreter(commandLineInterpreter)
    , _threadPool(threadPool)
    , _logger(logger)
{
}

bool RemoteAccessServer::start()
{
	_remoteControllersHandler = std::make_shared<RemoteControllersHandler>(_interpreter, _threadPool, _logger);

	for (const auto& configuration : _remoteAccessConfigurations)
	{
		// Create the remote access server for this configuration and check that it worked
		auto server = _connectionManager->createServer(configuration);
		if (!server)
		{
			GHOST_ERROR(_logger) << "Failed registering a remote access server.";
			stop();
			return false;
		}

		// Sets the remote controller handler to process clients of this server
		server->setClientHandler(_remoteControllersHandler);

		// Start the server, it may now receive clients
		if (!server->start())
		{
			GHOST_ERROR(_logger) << "Failed starting a remote access server.";
			stop();
			return false;
		}

		// Add the server to the list of remote controller servers.
		_remoteAccessServers.push_back(server);
	}

	return true;
}

void RemoteAccessServer::stop()
{
	for (const auto& server : _remoteAccessServers) server->stop();
	_remoteAccessServers.clear();
	_remoteControllersHandler.reset();
}
