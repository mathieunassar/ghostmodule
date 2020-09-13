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
#include <thread>

const std::string ConnectionMonkeyTest::TEST_NAME = "ConnectionMonkey";

ConnectionMonkeyTest::ConnectionMonkeyTest(const std::shared_ptr<ghost::ThreadPool>& threadPool,
					   const std::shared_ptr<ghost::Logger>& logger)
    : Systemtest(threadPool, logger)
    , _lastSentId(0)
    , _minPort(7600)
    , _maxPort(7610)
    , _maxConnections(100)
    , _generator(std::random_device().operator()())
    , _publishersCreated(0)
    , _subscribersCreated(0)
    , _publishersKilled(0)
    , _subscribersKilled(0)
    , _waitedTime(0)
{
}

bool ConnectionMonkeyTest::setUp()
{
	GHOST_INFO(_logger) << "     _mm_ ";
	GHOST_INFO(_logger) << "_  c( oo )-   _";
	GHOST_INFO(_logger) << " \\    (_)    /";
	GHOST_INFO(_logger) << "  \\____|____/";

	_connectionManager = ghost::ConnectionManager::create();
	ghost::ConnectionGRPC::initialize(_connectionManager, _threadPool);

	_lastSentId = 0;

	// populate the action map
	_actions.clear();
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::sleepAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::createPublisherAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::createSubscriberAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::sendMessageAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::killPublisherAction, this));
	_actions.emplace_back(std::bind(&ConnectionMonkeyTest::killSubscriberAction, this));

	return true;
}

void ConnectionMonkeyTest::tearDown()
{
	_connectionManager.reset();

	_publisherWriters.clear();
	_publishers.clear();
	_subscriberWriters.clear();
	_subscribers.clear();

	_actions.clear();
}

bool ConnectionMonkeyTest::run()
{
	std::uniform_int_distribution<> distribution(0, _actions.size() - 1);

	auto state = getState();
	while (state == State::EXECUTING && checkTestDuration())
	{
		// Randomly determine the next action to execute
		int nextAction = distribution(_generator);
		// execute the action and assert its success
		bool actionResult = _actions[nextAction]();
		require(actionResult);

		// loop again
		state = getState();
	}

	return true;
}

void ConnectionMonkeyTest::onPrintSummary() const
{
	GHOST_INFO(_logger) << "Publishers created: " << _publishersCreated;
	GHOST_INFO(_logger) << "Subscribers created: " << _subscribersCreated;
	GHOST_INFO(_logger) << "Publishers killed: " << _publishersKilled;
	GHOST_INFO(_logger) << "Subscribers killed: " << _subscribersKilled;
	GHOST_INFO(_logger) << "Messages sent: " << _lastSentId;
	GHOST_INFO(_logger) << "Time waited: " << _waitedTime;
}

bool ConnectionMonkeyTest::sleepAction()
{
	auto sleepDistribution = std::uniform_int_distribution<>(0, 1000);
	int sleepTime = sleepDistribution(_generator);
	_waitedTime += sleepTime;

	GHOST_INFO(_logger) << "sleepAction for " << sleepTime << " ms.";
	std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

	return true;
}

bool ConnectionMonkeyTest::createPublisherAction()
{
	if (hasEnoughConnections()) return true;

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
		require(!startResult);
		publisher->stop();
		GHOST_INFO(_logger) << "createPublisherAction on port " << chosenPort
				    << ": a publisher was already started for that port.";
	}
	else
	{
		require(startResult);
		_publisherWriters[chosenPort] = publisher->getWriter<google::protobuf::StringValue>();
		_publishers[chosenPort] = publisher;
		_publishersCreated++;
		GHOST_INFO(_logger) << "createPublisherAction on port " << chosenPort
				    << ": started new publisher - waiting 500 ms for setup";
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return true;
}

bool ConnectionMonkeyTest::createSubscriberAction()
{
	if (hasEnoughConnections()) return true;

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
		require(startResult);
		_subscriberWriters[chosenPort].push_back(subscriber->getReader<google::protobuf::StringValue>());
		_subscribers[chosenPort].push_back(subscriber);
		_subscribersCreated++;
		GHOST_INFO(_logger) << "createSubscriberAction on port " << chosenPort
				    << ": started new subscriber - waiting 500 ms for setup";
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	else
	{
		require(!startResult);
		subscriber->stop();
		GHOST_INFO(_logger) << "createSubscriberAction on port " << chosenPort
				    << ": no publisher was started for that port.";
	}

	return true;
}

bool ConnectionMonkeyTest::sendMessageAction()
{
	if (_publisherWriters.size() == 0) return true;

	auto publisherMapDistribution = std::uniform_int_distribution<>(0, _publisherWriters.size() - 1);
	auto publisher = _publisherWriters.begin();
	std::advance(publisher, publisherMapDistribution(_generator));

	GHOST_INFO(_logger) << "sendMessageAction for publisher on port: " << publisher->first << ".";

	auto message = google::protobuf::StringValue::default_instance();
	message.set_value(std::to_string(_lastSentId));

	bool writeResult = publisher->second->write(message);
	require(writeResult);

	const auto& subscribers = _subscriberWriters[publisher->first];
	for (const auto& subscriber : subscribers)
	{
		auto readMessage = google::protobuf::StringValue::default_instance();
		bool readResult = subscriber->read(readMessage);
		require(readResult);
		require(std::stoll(readMessage.value()) == _lastSentId);
	}

	GHOST_INFO(_logger) << "sendMessageAction for publisher on port: " << publisher->first << ": sent to "
			    << subscribers.size() << " subscribers.";
	_lastSentId++;
	return true;
}

bool ConnectionMonkeyTest::killPublisherAction()
{
	if (_publishers.size() == 0) return true;

	if (!shouldReallyDoIt(5)) // 5% execution chance
		return true;

	auto publisherMapDistribution = std::uniform_int_distribution<>(0, _publishers.size() - 1);
	auto publisher = _publishers.begin();
	std::advance(publisher, publisherMapDistribution(_generator));

	int chosenPort = publisher->first;
	GHOST_INFO(_logger) << "killPublisherAction for publisher on port: " << chosenPort << ".";

	bool stopResult = publisher->second->stop();
	require(stopResult);

	_publishers.erase(chosenPort);
	size_t publishersErased = _publisherWriters.erase(chosenPort);
	_publishersKilled++;

	// check the subscribers?
	const auto& subscribers = _subscribers[chosenPort];
	for (const auto& subscriber : subscribers) subscriber->stop();

	_subscriberWriters.erase(chosenPort);
	size_t subscribersErased = _subscribers.erase(chosenPort);

	GHOST_INFO(_logger) << "killPublisherAction for publisher on port: " << chosenPort << ": erased "
			    << publishersErased << " publishers and " << subscribersErased << " subscribers.";
	return true;
}

bool ConnectionMonkeyTest::killSubscriberAction()
{
	if (_subscribers.size() == 0) return true;

	if (!shouldReallyDoIt(10)) // 10% execution chance
		return true;

	auto subscriberMapDistribution = std::uniform_int_distribution<>(0, _subscribers.size() - 1);
	auto subscriberList = _subscribers.begin();
	std::advance(subscriberList, subscriberMapDistribution(_generator));

	int chosenPort = subscriberList->first;
	GHOST_INFO(_logger) << "killSubscriberAction for subscriber on port: " << chosenPort << ".";

	if (subscriberList->second.size() == 0) return true;

	subscriberMapDistribution = std::uniform_int_distribution<>(0, subscriberList->second.size() - 1);
	int subscriberIndex = subscriberMapDistribution(_generator);

	bool stopResult = subscriberList->second[subscriberIndex]->stop();
	require(stopResult);
	_subscribersKilled++;

	// Erase the deleted subscriber
	subscriberList->second.erase(subscriberList->second.begin() + subscriberIndex);
	_subscriberWriters[chosenPort].erase(_subscriberWriters[chosenPort].begin() + subscriberIndex);

	GHOST_INFO(_logger) << "killSubscriberAction for subscriber on port: " << chosenPort << ": erased subcriber.";
	return true;
}

bool ConnectionMonkeyTest::shouldReallyDoIt(int percentage)
{
	auto chanceDistribution = std::uniform_int_distribution<>(0, 100);
	int randomNumber = chanceDistribution(_generator);
	return randomNumber < percentage;
}

bool ConnectionMonkeyTest::hasEnoughConnections() const
{
	int connectionsCount = _publishers.size();
	for (auto it = _subscribers.begin(); it != _subscribers.end(); ++it) connectionsCount += it->second.size();

	return connectionsCount >= _maxConnections;
}

std::string ConnectionMonkeyTest::getName() const
{
	return TEST_NAME;
}
