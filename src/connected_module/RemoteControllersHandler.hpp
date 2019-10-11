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

#ifndef GHOST_INTERNAL_REMOTECONTROLLERSHANDLER_HPP
#define GHOST_INTERNAL_REMOTECONTROLLERSHANDLER_HPP

#include <ghost/connection/ClientHandler.hpp>
#include <ghost/module/CommandLineInterpreter.hpp>
#include "RemoteController.hpp"

namespace ghost
{
namespace internal
{
/**
 *	This handler for server clients creates a ghost::internal::RemoteController
 *	for each new client. The controller contains a ghost::internal::RemoteConsole
 *	which is fed by and feeds the ghost::Client connection, and an executor that
 *	can execute command lines with the provided ghost::CommandLineInterpreter.
 */
class RemoteControllersHandler : public ghost::ClientHandler
{
public:
	RemoteControllersHandler(const std::shared_ptr<ghost::CommandLineInterpreter>& commandLineInterpreter);

	// From ghost::ClientHandler
	void configureClient(const std::shared_ptr<ghost::Client>& client) override;
	bool handle(std::shared_ptr<ghost::Client> client, bool& keepClientAlive) override;

private:
	std::shared_ptr<ghost::CommandLineInterpreter> _interpreter;
	std::vector<std::shared_ptr<RemoteController>> _controllers;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_REMOTECONTROLLERSHANDLER_HPP
