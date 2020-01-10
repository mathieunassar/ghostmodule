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

#include "RemoteConsole.hpp"
#include <thread>

using namespace ghost::internal;

RemoteConsole::RemoteConsole(const std::shared_ptr<ghost::Client>& client) : _client(client)
{
	auto messageHandler = _client->addMessageHandler();
	messageHandler->addHandler<google::protobuf::StringValue>(
	    std::bind(&RemoteConsole::messageCallback, this, std::placeholders::_1));

	_writer = _client->getWriter<google::protobuf::StringValue>();
}

void RemoteConsole::start()
{
}

void RemoteConsole::stop()
{
	_client->stop();
}

void RemoteConsole::setPromptFormat(const std::string& prompt)
{
	// not yet supported - maybe tell the client to change its prompt??
}

void RemoteConsole::setInputMode(ghost::InputMode mode)
{
	// Nothing to do, it's parallel!
}

void RemoteConsole::setCommandCallback(std::function<void(const std::string&)> cmdCallback)
{
	auto messageHandler = _client->addMessageHandler();
	messageHandler->addHandler<google::protobuf::StringValue>(
	    [this, cmdCallback](const google::protobuf::StringValue& msg) {
		    messageCallback(msg);
		    cmdCallback(msg.value());
	    });
}

void RemoteConsole::write(const std::string& line)
{
	auto msg = google::protobuf::StringValue::default_instance();
	msg.set_value(line);
	_writer->write(msg);
}

std::string RemoteConsole::getLine()
{
	// If a command is already available, return it
	if (hasCommands()) return getCommand();

	// Otherwise wait that one arrives and return it
	while (!hasCommands())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// If the client died, this method returns an empty string
		if (!_client->isRunning()) return "";
	}
	return getCommand();
}

void RemoteConsole::flush()
{
	// Nothing to do
}

bool RemoteConsole::hasCommands() const
{
	std::unique_lock<std::mutex> lock(_commandQueueLock);
	return _messages.size() != 0 && _client->isRunning();
}

std::string RemoteConsole::getCommand()
{
	std::unique_lock<std::mutex> lock(_commandQueueLock);
	if (_messages.size() == 0) throw std::logic_error("There are no commands from the RemoteConsole.");

	auto msg = _messages.front();
	_messages.pop_front();
	return msg;
}

void RemoteConsole::messageCallback(const google::protobuf::StringValue& message)
{
	std::unique_lock<std::mutex> lock(_commandQueueLock);
	_messages.push_back(message.value());
}
