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

#ifndef GHOST_INTERNAL_REMOTECONTROLLER_HPP
#define GHOST_INTERNAL_REMOTECONTROLLER_HPP

#include <ghost/connection/Client.hpp>
#include <ghost/module/CommandLineInterpreter.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include "RemoteConsole.hpp"

namespace ghost
{
namespace internal
{
// NEXT: create an executor that sets a command callback and executes command lines
// - the callback will just notify but still store the messages in the console
// - the executor then gets a command and execute it
// - the executor does not execute commands that came while a command was being executed
// -
class RemoteController
{
public:
	RemoteController(const std::shared_ptr<ghost::Client>& client,
			 const std::shared_ptr<ghost::CommandLineInterpreter>& commandLineInterpreter);
	~RemoteController();

	void commandCallback(const std::string& command);

private:
	enum class State
	{
		IDLE,
		EXECUTING
	};

	std::shared_ptr<RemoteConsole> _console;
	std::shared_ptr<ghost::CommandLineInterpreter> _interpreter;
	std::thread _executor;
	State _state;
	std::mutex _mutex;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_REMOTECONTROLLER_HPP
