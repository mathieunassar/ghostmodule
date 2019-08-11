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

#include "SubscriberGRPC.hpp"

using namespace ghost::internal;

SubscriberGRPC::SubscriberGRPC(const ghost::ConnectionConfiguration& config)
	: SubscriberGRPC(ghost::NetworkConnectionConfiguration::initializeFrom(config))
{

}

SubscriberGRPC::SubscriberGRPC(const ghost::NetworkConnectionConfiguration& config)
	: ghost::Subscriber(config)
	, _client(makeNoWriterConfig(config))
{

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

ghost::NetworkConnectionConfiguration SubscriberGRPC::makeNoWriterConfig(const ghost::NetworkConnectionConfiguration& config)
{
	ghost::NetworkConnectionConfiguration newConfig(config);
	ghost::ConfigurationValue noWriterValue;
	noWriterValue.write(true);
	newConfig.getConfiguration()->addAttribute(BASE_CLIENT_GRPC_CONFIG_NOWRITER, noWriterValue, true);
	return newConfig;
}
