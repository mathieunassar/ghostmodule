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

#ifndef GHOST_INTERNAL_REMOTEACCESSSERVER_HPP
#define GHOST_INTERNAL_REMOTEACCESSSERVER_HPP

#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/module/Logger.hpp>
#include <ghost/module/ThreadPool.hpp>
#include <memory>
#include <vector>
#include "RemoteControllersHandler.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Server for the remote access feature.
 *	It handles zero, one or multiple ghost::Server to receive clients which
 *	are the remote controllers.
 *	The messages received by the clients are provided to the ghost::CommandLineInterpreter,
 *	and a ghost::Console implementation is bound to send messages to those clients.
 *	This bi-directional communication simulates a real local console.
 *
 *	See ghost::internal::RemoteControlClient for the client side handling of this feature.
 */
class RemoteAccessServer
{
public:
	RemoteAccessServer(const std::vector<ghost::ConnectionConfiguration>& configurations,
			   const std::shared_ptr<ghost::ConnectionManager>& connectionManager,
			   const std::shared_ptr<ghost::CommandLineInterpreter>& commandLineInterpreter,
			   const std::shared_ptr<ghost::ThreadPool>& threadPool,
			   const std::shared_ptr<ghost::Logger>& logger);

	bool start();
	void stop();

private:
	std::vector<ghost::ConnectionConfiguration> _remoteAccessConfigurations;
	std::vector<std::shared_ptr<ghost::Server>> _remoteAccessServers;
	std::shared_ptr<RemoteControllersHandler> _remoteControllersHandler;
	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
	std::shared_ptr<ghost::CommandLineInterpreter> _interpreter;
	std::shared_ptr<ghost::ThreadPool> _threadPool;
	std::shared_ptr<ghost::Logger> _logger;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_REMOTEACCESSSERVER_HPP
