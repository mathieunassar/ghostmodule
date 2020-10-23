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

#include "SubscriberGRPC.hpp"

using namespace ghost::internal;

SubscriberGRPC::SubscriberGRPC(const ghost::ConnectionConfiguration& config,
			       const std::shared_ptr<ghost::ThreadPool>& threadPool)
    : SubscriberGRPC(ghost::NetworkConnectionConfiguration::initializeFrom(config), threadPool)
{
}

SubscriberGRPC::SubscriberGRPC(const ghost::NetworkConnectionConfiguration& config,
			       const std::shared_ptr<ghost::ThreadPool>& threadPool)
    : ghost::Subscriber(config)
    , _client(threadPool, config.getServerIpAddress(), config.getServerPortNumber(), config.getThreadPoolSize())
{
	_client.setReaderSink(getReaderSink());
}

bool SubscriberGRPC::start()
{
	return _client.start();
}

bool SubscriberGRPC::stop()
{
	return _client.stop();
}

bool SubscriberGRPC::isRunning() const
{
	return _client.isRunning();
}
