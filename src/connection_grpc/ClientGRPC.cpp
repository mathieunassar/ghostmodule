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

#include "ClientGRPC.hpp"

using namespace ghost::internal;

ClientGRPC::ClientGRPC(const ghost::ConnectionConfiguration& config,
		       const std::shared_ptr<ghost::ThreadPool>& threadPool)
    : ClientGRPC(ghost::NetworkConnectionConfiguration::initializeFrom(config), threadPool)
{
}

ClientGRPC::ClientGRPC(const ghost::NetworkConnectionConfiguration& config,
		       const std::shared_ptr<ghost::ThreadPool>& threadPool)
    : ghost::Client(config)
    , _client(threadPool, config.getServerIpAddress(), config.getServerPortNumber(), config.getThreadPoolSize())
{
	_client.setReaderSink(getReaderSink());
	_client.setWriterSink(getWriterSink());
}

bool ClientGRPC::start()
{
	return _client.start();
}

bool ClientGRPC::stop()
{
	return _client.stop();
}

bool ClientGRPC::isRunning() const
{
	return _client.isRunning();
}
