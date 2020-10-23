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

#include "PublisherGRPC.hpp"

using namespace ghost::internal;

PublisherGRPC::PublisherGRPC(const ghost::ConnectionConfiguration& config,
			     const std::shared_ptr<ghost::ThreadPool>& threadPool)
    : PublisherGRPC(ghost::NetworkConnectionConfiguration::initializeFrom(config), threadPool)
{
}

PublisherGRPC::PublisherGRPC(const ghost::NetworkConnectionConfiguration& config,
			     const std::shared_ptr<ghost::ThreadPool>& threadPool)
    : ghost::Publisher(config), _threadPool(threadPool), _server(config, threadPool)
{
	_handler = std::make_shared<PublisherClientHandler>();
	_server.setClientHandler(_handler);
}

bool PublisherGRPC::start()
{
	if (!_executor)
	{
		_executor = _threadPool->makeScheduledExecutor();
		_executor->scheduleAtFixedRate(std::bind(&PublisherGRPC::writerThread, this),
					       std::chrono::milliseconds(10));

		return _server.start();
	}
	return false;
}

bool PublisherGRPC::stop()
{
	getWriterSink()->drain();

	if (_executor) _executor->stop();

	_handler->releaseClients();
	return _server.stop();
}

bool PublisherGRPC::isRunning() const
{
	return _server.isRunning();
}

size_t PublisherGRPC::countSubscribers() const
{
	return _handler->countSubscribers();
}

void PublisherGRPC::writerThread()
{
	auto writer = getWriterSink();
	google::protobuf::Any message;
	bool getSuccess = true;
	while (getSuccess)
	{
		getSuccess = writer->get(message, std::chrono::milliseconds(0));
		if (getSuccess)
		{
			_handler->send(message);
			writer->pop();
		}
	}
}
