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

#ifndef GHOST_TESTS_CONNECTIONMONKEYTEST_HPP
#define GHOST_TESTS_CONNECTIONMONKEYTEST_HPP

#include <google/protobuf/wrappers.pb.h>

#include <functional>
#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection/Reader.hpp>
#include <ghost/connection/Writer.hpp>
#include <random>
#include <vector>

#include "Systemtest.hpp"

class ConnectionMonkeyTest : public Systemtest
{
public:
	ConnectionMonkeyTest(const std::shared_ptr<ghost::ThreadPool>& threadPool,
			     const std::shared_ptr<ghost::Logger>& logger);

	std::string getName() const override;

private:
	bool setUp() override;
	void tearDown() override;
	bool run() override;
	void onPrintSummary() const override;

	bool sleepAction();
	bool createPublisherAction();
	bool createSubscriberAction();
	bool sendMessageAction();
	bool killPublisherAction();
	bool killSubscriberAction();

	bool shouldReallyDoIt(int percentage);
	bool hasEnoughConnections() const;

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
	int _maxConnections;

	// random generator
	std::mt19937 _generator;

	// statistics
	long long _publishersCreated;
	long long _subscribersCreated;
	long long _publishersKilled;
	long long _subscribersKilled;
	long long _waitedTime;
};

#endif // GHOST_TESTS_CONNECTIONMONKEYTEST_HPP
