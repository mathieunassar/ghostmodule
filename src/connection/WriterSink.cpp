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

#include "WriterSink.hpp"

using namespace ghost::internal;

WriterSink::WriterSink() : _drained(false)
{
}

bool WriterSink::get(google::protobuf::Any& message, std::chrono::milliseconds timeout)
{
	if (_drained) return false;

	if (timeout == std::chrono::milliseconds(-1))
	{
		message = getMessageQueue()->get();
		return true;
	}

	return getMessageQueue()->tryGet(timeout, message);
}

void WriterSink::pop()
{
	getMessageQueue()->pop();
}

void WriterSink::drain()
{
	_drained = true;
}

bool WriterSink::push(const google::protobuf::Any& message, bool blocking)
{
	if (_drained) return false;

	getMessageQueue()->push(message);

	if (blocking)
	{
		while (getMessageQueue()->size() != 0 &&
		       !_drained) // blocks until the message is gone, use blocking queue instead!
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	return true;
}
