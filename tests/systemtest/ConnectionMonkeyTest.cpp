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

#include "ConnectionMonkeyTest.hpp"
#include <ghost/connection_grpc/ConnectionGRPC.hpp>

const std::string ConnectionMonkeyTest::TEST_NAME = "ConnectionMonkey";

ConnectionMonkeyTest::ConnectionMonkeyTest(const std::shared_ptr<ghost::Logger>& logger)
	: Systemtest(logger)
	, _connectionManager(ghost::ConnectionManager::create())
	, _lastSentId(0)
	, _minPort(7600)
	, _maxPort(7610)
	, _generator(std::random_device().operator()())
	, _distribution(std::uniform_int_distribution<>(0, 0))
{
	ghost::ConnectionGRPC::initialize(_connectionManager);	
}

bool ConnectionMonkeyTest::setUp()
{
	GHOST_INFO(_logger) << "     _mm_ ";
	GHOST_INFO(_logger) << "_  c( oo )-   _";
	GHOST_INFO(_logger) << " \\    (_)    /";
	GHOST_INFO(_logger) << "  \\____|____/";

	// populate the action map
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::sleepAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::createPublisherAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::createSubscriberAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::sendMessageAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::killPublisherAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::killSubscriberAction, this));
	
	// initialize the distribution
	_distribution = std::uniform_int_distribution<>(0, _actions.size() - 1);

	return true;
}

bool ConnectionMonkeyTest::run()
{
	auto state = getState();
	while (state == State::EXECUTING && checkTestDuration())
	{
		// Randomly determine the next action to execute
		int nextAction = _distribution(_generator);
		// execute the action and assert its success
		bool actionResult = _actions[nextAction]();
		EXPECT_TRUE(actionResult);

		// loop again
		state = getState();
	}

	return true;
}

bool ConnectionMonkeyTest::sleepAction()
{
	auto sleepDistribution = std::uniform_int_distribution<>(0, 1000);
	int sleepTime = sleepDistribution(_generator);

	GHOST_INFO(_logger) << "sleepAction for " << sleepTime << " ms.";
	std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

	return true;
}

bool ConnectionMonkeyTest::createPublisherAction()
{
	auto portDistribution = std::uniform_int_distribution<>(_minPort, _maxPort);
	int chosenPort = portDistribution(_generator);

	GHOST_INFO(_logger) << "createPublisherAction on port " << chosenPort << ".";

	ghost::ConnectionConfigurationGRPC config;
	config.setServerIpAddress("127.0.0.1");
	config.setServerPortNumber(chosenPort);

	std::shared_ptr<ghost::Publisher> publisher = _connectionManager->createPublisher(config);
	bool startResult = publisher->start();

	// there is already a publisher there, the call should fail
	if (_publishers.find(chosenPort) != _publishers.end())
	{
		EXPECT_FALSE(startResult);
		publisher->stop();
		GHOST_INFO(_logger) << "createPublisherAction on port " << chosenPort << ": a publisher was already started for that port.";
	}
	else
	{
		EXPECT_TRUE(startResult);
		_publisherWriters[chosenPort] = publisher->getWriter<google::protobuf::StringValue>();
		_publishers[chosenPort] = publisher;
		GHOST_INFO(_logger) << "createPublisherAction on port " << chosenPort << ": started new publisher - waiting 200 ms for setup";
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	return true;
}

bool ConnectionMonkeyTest::createSubscriberAction()
{
	auto portDistribution = std::uniform_int_distribution<>(_minPort, _maxPort);
	int chosenPort = portDistribution(_generator);

	GHOST_INFO(_logger) << "createSubscriberAction on port " << chosenPort << ".";

	ghost::ConnectionConfigurationGRPC config;
	config.setServerIpAddress("127.0.0.1");
	config.setServerPortNumber(chosenPort);

	std::shared_ptr<ghost::Subscriber> subscriber = _connectionManager->createSubscriber(config);
	bool startResult = subscriber->start();

	// if there is a publisher there, the call should succeed
	if (_publishers.find(chosenPort) != _publishers.end())
	{
		EXPECT_TRUE(startResult);
		_subscriberWriters[chosenPort].push_back(subscriber->getReader<google::protobuf::StringValue>());
		_subscribers[chosenPort].push_back(subscriber);
		GHOST_INFO(_logger) << "createSubscriberAction on port " << chosenPort << ": started new subscriber - waiting 200 ms for setup";
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	else
	{
		EXPECT_FALSE(startResult);
		subscriber->stop();
		GHOST_INFO(_logger) << "createSubscriberAction on port " << chosenPort << ": no publisher was started for that port.";
	}

	return true;
}

bool ConnectionMonkeyTest::sendMessageAction()
{
	if (_publisherWriters.size() == 0)
		return true;

	auto publisherMapDistribution = std::uniform_int_distribution<>(0, _publisherWriters.size() - 1);
	auto publisher = _publisherWriters.begin();
	std::advance(publisher, publisherMapDistribution(_generator));

	GHOST_INFO(_logger) << "sendMessageAction for publisher on port: " << publisher->first << ".";

	auto message = google::protobuf::StringValue::default_instance();
	message.set_value(std::to_string(_lastSentId));

	bool writeResult = publisher->second->write(message);
	EXPECT_TRUE(writeResult);

	const auto& subscribers = _subscriberWriters[publisher->first];
	for (const auto& subscriber : subscribers)
	{
		auto readMessage = google::protobuf::StringValue::default_instance();
		bool readResult = subscriber->read(readMessage);
		EXPECT_TRUE(readResult);
		EXPECT_TRUE(std::stoll(readMessage.value()) == _lastSentId);
	}

	GHOST_INFO(_logger) << "sendMessageAction for publisher on port: " << publisher->first << ": sent to " << subscribers.size() << " subscribers.";
	_lastSentId++;
	return true;
}

bool ConnectionMonkeyTest::killPublisherAction()
{
	if (_publishers.size() == 0)
		return true;

	if (!shouldReallyDoIt())
		return true;

	auto publisherMapDistribution = std::uniform_int_distribution<>(0, _publishers.size() - 1);
	auto publisher = _publishers.begin();
	std::advance(publisher, publisherMapDistribution(_generator));

	int chosenPort = publisher->first;
	GHOST_INFO(_logger) << "killPublisherAction for publisher on port: " << chosenPort << ".";

	bool stopResult = publisher->second->stop();
	EXPECT_TRUE(stopResult);

	_publishers.erase(chosenPort);
	size_t publishersErased = _publisherWriters.erase(chosenPort);

	// check the subscribers?
	_subscriberWriters.erase(chosenPort);
	size_t subscribersErased = _subscribers.erase(chosenPort);

	GHOST_INFO(_logger) << "killPublisherAction for publisher on port: " << chosenPort << ": erased " << publishersErased << " publishers and "
		<< subscribersErased << " subscribers.";
	return true;
}

bool ConnectionMonkeyTest::killSubscriberAction()
{
	if (_subscribers.size() == 0)
		return true;

	if (!shouldReallyDoIt())
		return true;

	auto subscriberMapDistribution = std::uniform_int_distribution<>(0, _subscribers.size() - 1);
	auto subscriberList = _subscribers.begin();
	std::advance(subscriberList, subscriberMapDistribution(_generator));

	int chosenPort = subscriberList->first;
	GHOST_INFO(_logger) << "killSubscriberAction for subscriber on port: " << chosenPort << ".";

	if (subscriberList->second.size() == 0)
		return true;

	subscriberMapDistribution = std::uniform_int_distribution<>(0, subscriberList->second.size() - 1);
	int subscriberIndex = subscriberMapDistribution(_generator);

	bool stopResult = subscriberList->second[subscriberIndex]->stop();
	EXPECT_TRUE(stopResult);

	// Erase the deleted subscriber
	subscriberList->second.erase(subscriberList->second.begin() + subscriberIndex);
	_subscriberWriters[chosenPort].erase(_subscriberWriters[chosenPort].begin() + subscriberIndex);

	GHOST_INFO(_logger) << "killSubscriberAction for subscriber on port: " << chosenPort << ": erased subcriber.";
	return true;
}

bool ConnectionMonkeyTest::shouldReallyDoIt()
{
	auto chanceDistribution = std::uniform_int_distribution<>(0, 100);
	int randomNumber = chanceDistribution(_generator);
	return randomNumber == 0;
}

std::string ConnectionMonkeyTest::getName() const
{
	return TEST_NAME;
}
