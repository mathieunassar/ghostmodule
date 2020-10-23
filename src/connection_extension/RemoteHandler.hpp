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

#ifndef GHOST_INTERNAL_REMOTEHANDLER_HPP
#define GHOST_INTERNAL_REMOTEHANDLER_HPP

#include <future>
#include <ghost/connection/Client.hpp>
#include <ghost/module/CommandLineInterpreter.hpp>
#include <ghost/module/Session.hpp>
#include <ghost/module/ThreadPool.hpp>
#include <memory>
#include <mutex>
#include <string>

#include "RemoteConsole.hpp"

namespace ghost
{
namespace internal
{
class RemoteHandler
{
public:
	RemoteHandler(const std::shared_ptr<ghost::Client>& client,
		      const std::shared_ptr<ghost::ThreadPool>& threadPool,
		      const std::shared_ptr<ghost::CommandLineInterpreter>& commandLineInterpreter);
	~RemoteHandler();

	bool isActive() const;
	void commandCallback(const std::string& command);

private:
	enum class State
	{
		IDLE,
		EXECUTING
	};

	std::shared_ptr<ghost::Client> _client;
	std::shared_ptr<RemoteConsole> _console;
	std::shared_ptr<ghost::CommandLineInterpreter> _interpreter;
	std::shared_ptr<ghost::Session> _session;
	std::shared_ptr<ghost::ThreadPool> _threadPool;
	std::future<void> _execution;
	State _state;
	mutable std::mutex _mutex;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_REMOTEHANDLER_HPP
