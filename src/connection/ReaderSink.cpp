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

#include "ReaderSink.hpp"

using namespace ghost::internal;

ReaderSink::ReaderSink()
	: _drained(false)
{

}

bool ReaderSink::put(const google::protobuf::Any& message)
{
	if (_drained)
		return false;
	
	if (_messageHandler) // if there is a message handler, don't use the read queue
		_messageHandler->handle(message);
	else
		getMessageQueue()->push(message);

	return true;
}

std::shared_ptr<ghost::MessageHandler> ReaderSink::addMessageHandler()
{
	_messageHandler = std::make_shared<ghost::internal::MessageHandler>();
	return _messageHandler;
}

void ReaderSink::drain()
{
	_drained = true;
}

bool ReaderSink::get(google::protobuf::Any& message, bool blocking)
{
	if (_drained)
		return false;

	if (_messageHandler)
		return false;

	// call is non blocking and there is nothing to read, return false.
	// User can find out that there is no issue by calling "isRunnung()" on the connection
	if (!blocking && getMessageQueue()->size() == 0)
		return false;

	// else, connection is blocking or there is something to read
	message = getMessageQueue()->get(); // copy is called here
	getMessageQueue()->pop();
	return true;
}
