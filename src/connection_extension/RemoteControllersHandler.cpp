/*
 * Copyright 2020 Mathieu Nassar
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

#include "RemoteControllersHandler.hpp"

using namespace ghost::internal;

RemoteControllersHandler::RemoteControllersHandler(
    const std::shared_ptr<ghost::CommandLineInterpreter>& commandLineInterpreter,
    const std::shared_ptr<ghost::ThreadPool>& threadPool, const std::shared_ptr<ghost::Logger>& logger)
    : _interpreter(commandLineInterpreter), _threadPool(threadPool), _logger(logger)
{
}

void RemoteControllersHandler::configureClient(const std::shared_ptr<ghost::Client>& client)
{
	GHOST_INFO(_logger) << "Remote controller connected.";
	purgeInactiveClients();
	auto controller = std::make_shared<RemoteHandler>(client, _threadPool, _interpreter);
	_controllers.push_back(controller);
}

bool RemoteControllersHandler::handle(std::shared_ptr<ghost::Client> client, bool& keepClientAlive)
{
	keepClientAlive = true;
	return true;
}

void RemoteControllersHandler::purgeInactiveClients()
{
	auto it = _controllers.begin();
	while (it != _controllers.end())
	{
		if (!(*it)->isActive())
		{
			it = _controllers.erase(it);
			GHOST_INFO(_logger) << "Inactive remote controller disconnected.";
		}
		else
			++it;
	}
}
