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

#include "RemoteHandler.hpp"
#include <ghost/module/CommandExecutionContext.hpp>

using namespace ghost::internal;

RemoteHandler::RemoteHandler(const std::shared_ptr<ghost::Client>& client,
			     const std::shared_ptr<ghost::CommandLineInterpreter>& commandLineInterpreter)
    : _console(std::make_shared<RemoteConsole>(client))
    , _state(State::IDLE)
    , _interpreter(commandLineInterpreter)
    , _session(ghost::Session::create())
{
	_console->start();
	_console->setCommandCallback(std::bind(&RemoteHandler::commandCallback, this, std::placeholders::_1));
}

RemoteHandler::~RemoteHandler()
{
	if (_executor.joinable()) _executor.join();
}

void RemoteHandler::commandCallback(const std::string& command)
{
	{ // Only enter this function if the state is IDLE once
		std::lock_guard<std::mutex> lock(_mutex);
		if (_state == State::EXECUTING) return;
		_state = State::EXECUTING;
	}
	// Wait for the end of the previous command's execution
	if (_executor.joinable()) _executor.join();

	// Get the command from the console and execute it in the executor thread
	auto cmd = _console->getCommand();
	_executor = std::thread([&]() {
		ghost::CommandExecutionContext context(_session);
		context.setConsole(_console);
		_interpreter->execute(cmd);
		_state = State::IDLE;
	});
}
