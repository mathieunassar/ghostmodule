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

#include "PublisherGRPC.hpp"

using namespace ghost::internal;

PublisherGRPC::PublisherGRPC(const ghost::NetworkConnectionConfiguration& config)
	: ghost::Publisher(config)
	, _server(config)
	, _writerThreadEnable(false)
{
	_handler = std::make_shared<PublisherClientHandler>();
	_server.setClientHandler(_handler);
}

bool PublisherGRPC::start()
{
	_writerThreadEnable = true;
	_writerThread = std::thread(&PublisherGRPC::writerThread, this);

	return _server.start();
}

bool PublisherGRPC::stop()
{
	_writerThreadEnable = false;
	if (_writerThread.joinable())
		_writerThread.join();

	_handler->releaseClients();
	return _server.stop();
}

bool PublisherGRPC::isRunning() const
{
	return _server.isRunning();
}

void PublisherGRPC::writerThread()
{
	while (_writerThreadEnable)
	{
		auto writer = getWriterSink();
		google::protobuf::Any message;
		bool getSuccess = writer->get(message, std::chrono::milliseconds(10));

		if (getSuccess)
		{
			_handler->send(message);
			writer->pop();
		}
	}
}
