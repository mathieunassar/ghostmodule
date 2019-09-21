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

#ifndef GHOST_TESTS_CONNECTIONMONKEYTEST_HPP
#define GHOST_TESTS_CONNECTIONMONKEYTEST_HPP

#include <vector>
#include <functional>
#include <random>
#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection/Writer.hpp>
#include <ghost/connection/Reader.hpp>
#include <google/protobuf/wrappers.pb.h>
#include "Systemtest.hpp"

class ConnectionMonkeyTest : public Systemtest
{
public:
	ConnectionMonkeyTest(const std::shared_ptr<ghost::Logger>& logger);

	bool setUp() override;
	void tearDown() override;
	bool run() override;
	std::string getName() const override;

private:
	bool sleepAction();
	bool createPublisherAction();
	bool createSubscriberAction();
	bool sendMessageAction();
	bool killPublisherAction();
	bool killSubscriberAction();

	bool shouldReallyDoIt();

	static const std::string TEST_NAME;

	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
	std::map<int, std::shared_ptr<ghost::Writer<google::protobuf::StringValue>>> _publisherWriters;
	std::map<int, std::shared_ptr<ghost::Publisher>> _publishers;
	std::map<int, std::vector<std::shared_ptr<ghost::Reader<google::protobuf::StringValue>>>> _subscriberWriters;
	std::map<int, std::vector<std::shared_ptr<ghost::Subscriber>>> _subscribers;
	long long _lastSentId;

	std::vector<std::function<bool()>> _actions;
	
	// port range configuration
	int _minPort;
	int _maxPort;

	// random generator
	std::mt19937 _generator;
	std::uniform_int_distribution<> _distribution;
};

#endif // GHOST_TESTS_CONNECTIONMONKEYTEST_HPP
