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

#ifndef GHOST_INTERNAL_REMOTECONTROLCLIENT_HPP
#define GHOST_INTERNAL_REMOTECONTROLCLIENT_HPP

#include <google/protobuf/wrappers.pb.h>
#include <ghost/connection/Client.hpp>
#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/module/CommandLineInterpreter.hpp>
#include <ghost/module/Console.hpp>
#include <ghost/module/Logger.hpp>
#include <memory>
#include <string>

namespace ghost
{
namespace internal
{
/**
 *	This class handles a ghost::Client that connects to a remote module in order to
 *	control it.
 *	The messages received by the client come from the remote module and are therefore
 *	displayed in the provided console, which is currently the console bound to the terminal
 *	of this module.
 *	Typing commands in the console of this module is intercepted by this class and filtered:
 *	- if the command begins with the value of PREFIX_LOCAL_EXECUTION ("local:"), the command
 *		is forwarded to the local ghost::CommandLineInterpreter;
 *	- otherwise, the commands are sent through the ghost::Client to be executed on the remote
 *		module.
 *
 *	See ghost::internal::RemoteAccessServer for the handling on the controlled module side.
 */
class RemoteControlClient
{
public:
	static const std::string PREFIX_LOCAL_EXECUTION;

	RemoteControlClient(const ghost::ConnectionConfiguration& configuration,
			    const std::shared_ptr<ghost::ConnectionManager>& connectionManager,
			    const std::shared_ptr<ghost::CommandLineInterpreter>& commandLineInterpreter,
			    const std::shared_ptr<ghost::Console>& console,
			    const std::shared_ptr<ghost::Logger>& logger);

	bool start();
	void stop();

private:
	void consoleCommandCallback(const std::string& command);
	void remoteMessageCallback(const google::protobuf::StringValue& message);

	ghost::ConnectionConfiguration _configuration;
	std::shared_ptr<ghost::Client> _remote;
	std::shared_ptr<ghost::Writer<google::protobuf::StringValue>> _remoteWriter;
	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
	std::shared_ptr<ghost::CommandLineInterpreter> _interpreter;
	std::shared_ptr<ghost::Console> _console;
	std::shared_ptr<ghost::Logger> _logger;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_REMOTECONTROLCLIENT_HPP
