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

#ifndef GHOST_INTERNAL_REMOTECONSOLE_HPP
#define GHOST_INTERNAL_REMOTECONSOLE_HPP

#include <google/protobuf/wrappers.pb.h>
#include <ghost/connection/Client.hpp>
#include <ghost/module/Console.hpp>
#include <list>
#include <memory>
#include <mutex>

namespace ghost
{
namespace internal
{
	// NEXT: create an executor that sets a command callback and executes command lines
	// - the callback will just notify but still store the messages in the console
	// - the executor then gets a command and execute it
	// - the executor does not execute commands that came while a command was being executed
	// - 
class RemoteConsole : public ghost::Console
{
public:
	RemoteConsole(const std::shared_ptr<ghost::Client>& client);

	// From ghost::Console
	void start() override;
	void stop() override;

	void setPromptFormat(const std::string& prompt) override;
	void setInputMode(ghost::InputMode mode) override;
	void setCommandCallback(std::function<void(const std::string&)> cmdCallback) override;
	void write(const std::string& line) override;
	std::string getLine() override;
	void flush() override;
	bool hasCommands() const override;
	std::string getCommand() override;

private:
	void messageCallback(const google::protobuf::StringValue& message);

	std::shared_ptr<ghost::Client> _client;
	std::shared_ptr<ghost::Writer<google::protobuf::StringValue>> _writer;
	mutable std::mutex _commandQueueLock;
	std::list<std::string> _messages;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_REMOTECONSOLE_HPP
