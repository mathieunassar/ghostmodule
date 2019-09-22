/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless ASSERT_TRUEd by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <thread>
#include <gtest/gtest.h>

#include <google/protobuf/wrappers.pb.h>
#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection_grpc/ConnectionGRPC.hpp>
#include <ghost/connection_grpc/ConnectionConfigurationGRPC.hpp>
#include <ghost/connection/NetworkConnectionConfiguration.hpp>
#include <ghost/connection/Writer.hpp>

#include "../connection/ConnectionTestUtils.hpp"

using namespace ghost;

using ::testing::_;

/**
 *	This test class groups the following test categories:
 *	- connection factory congiguration (API works)
 *	- Operation workflow of gRPC based connections
 *	- Connectivity server-client, subscriber-publisher wrt the connection API
 *	- Write/Read operations are possible
 */
// TODO: other test classes for RPC management, ClientManager(?)
class ConnectionGRPCTests : public testing::Test
{
protected:
	void SetUp() override
	{
		_connectionManager = ghost::ConnectionManager::create();
		ghost::ConnectionGRPC::initialize(_connectionManager, _config);

		_config.setServerPortNumber(TEST_PORT);

		_doubleValueMessageWasHandledCounter = 0;
		_doubleValueMessageWasHandledMap.clear();
		
		_clientsHandledCount = 0;
		_clientsHandledExpected = 0;
	}

	void TearDown() override
	{
		_connectionManager.reset();
	}

	void createServer(const ghost::NetworkConnectionConfiguration& config)
	{
		_server = _connectionManager->createServer(config);
		ASSERT_TRUE(_server);
		ASSERT_FALSE(_server->isRunning());
		_clientHandlerMock = std::make_shared<ClientHandlerMock>();
		_server->setClientHandler(_clientHandlerMock);
	}

	void startServer()
	{
		if (_server)
		{
			bool startResult = _server->start();
			ASSERT_TRUE(startResult);
			ASSERT_TRUE(_server->isRunning());
		}
	}

	void startClients(const ghost::NetworkConnectionConfiguration config, size_t count, bool standardExpectations = true)
	{
		_clientsHandledExpected = count;

		if (standardExpectations)
		{
			EXPECT_CALL(*_clientHandlerMock, configureClient(_)).Times(count);
			EXPECT_CALL(*_clientHandlerMock, handle(_, _)).Times(count)
				.WillRepeatedly([&](std::shared_ptr<ghost::Client>, bool&) { _clientsHandledCount++; return true; });
		}

		for (size_t i = 0; i < count; ++i)
		{
			auto client = _connectionManager->createClient(config);
			ASSERT_TRUE(client);
			bool startResult = client->start();
			ASSERT_TRUE(startResult);
			ASSERT_TRUE(client->isRunning());
			_clients.push_back(client);
		}
	}

	void waitForClientsHandled()
	{
		auto now = std::chrono::steady_clock::now();
		auto deadline = now + std::chrono::seconds(1);
		while (_clientsHandledCount < _clientsHandledExpected && now < deadline)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			now = std::chrono::steady_clock::now();
		}
	}

	void createPublisher(const ghost::NetworkConnectionConfiguration& config)
	{
		_publisher = _connectionManager->createPublisher(config);
		ASSERT_TRUE(_publisher);
		ASSERT_FALSE(_publisher->isRunning());
	}

	void startPublisher()
	{
		if (_publisher)
		{
			bool startResult = _publisher->start();
			ASSERT_TRUE(startResult);
			ASSERT_TRUE(_publisher->isRunning());
		}
	}

	void startSubscribers(const ghost::NetworkConnectionConfiguration config, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			auto subscriber = _connectionManager->createSubscriber(config);
			ASSERT_TRUE(subscriber);
			bool startResult = subscriber->start();
			ASSERT_TRUE(startResult);
			ASSERT_TRUE(subscriber->isRunning());
			_subscribers.push_back(subscriber);
		}
		// allow the publisher to register them
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	void setupSubscribers(int count)
	{
		for (int i = 0; i < count; ++i)
		{
			auto handler = _subscribers[i]->addMessageHandler();
			handler->addHandler<google::protobuf::DoubleValue>(
				std::bind(&ConnectionGRPCTests::doubleMessageMassHandler, this, i, std::placeholders::_1));
		}
	}

	void checkSubscribersReceivedMessages(int count)
	{
		for (int i = 0; i < count; ++i)
		{
			auto now = std::chrono::steady_clock::now();
			auto deadline = now + std::chrono::seconds(1);
			while (now < deadline && _doubleValueMessageWasHandledMap.find(i) == _doubleValueMessageWasHandledMap.end())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				now = std::chrono::steady_clock::now();
			}
			ASSERT_TRUE(_doubleValueMessageWasHandledMap.count(i) == 1);
			ASSERT_TRUE(_doubleValueMessageWasHandledMap[i] == 1);
		}
	}

	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
	ghost::ConnectionConfigurationGRPC _config;

	std::shared_ptr<ghost::Server> _server;
	std::vector<std::shared_ptr<ghost::Client>> _clients;
	std::shared_ptr<ClientHandlerMock> _clientHandlerMock;
	int _clientsHandledCount;
	int _clientsHandledExpected;

	std::shared_ptr<ghost::Publisher> _publisher;
	std::vector<std::shared_ptr<ghost::Subscriber>> _subscribers;

	int _doubleValueMessageWasHandledCounter;
	std::map<int, int> _doubleValueMessageWasHandledMap;
	
	static const int TEST_PORT;

public:
	void doubleMessageHandler(const google::protobuf::DoubleValue& message)
	{
		_doubleValueMessageWasHandledCounter++;
	}

	void doubleMessageMassHandler(int id, const google::protobuf::DoubleValue& message)
	{
		if (_doubleValueMessageWasHandledMap.find(id) == _doubleValueMessageWasHandledMap.end())
			_doubleValueMessageWasHandledMap[id] = 1;
		else
			_doubleValueMessageWasHandledMap[id]++;
	}
};

const int ConnectionGRPCTests::TEST_PORT = 5678;

TEST_F(ConnectionGRPCTests, test_ConnectionGRPC_populatesConnectionManagerWithServerRule)
{
	auto server = _connectionManager->createServer(_config);
	ASSERT_TRUE(server);
}

TEST_F(ConnectionGRPCTests, test_ConnectionGRPC_populatesConnectionManagerWithClientRule)
{
	auto client = _connectionManager->createClient(_config);
	ASSERT_TRUE(client);
}

TEST_F(ConnectionGRPCTests, test_ConnectionGRPC_populatesConnectionManagerWithPublisherRule)
{
	auto publisher = _connectionManager->createPublisher(_config);
	ASSERT_TRUE(publisher);
}

TEST_F(ConnectionGRPCTests, test_ConnectionGRPC_populatesConnectionManagerWithSubscriberRule)
{
	auto subscriber = _connectionManager->createSubscriber(_config);
	ASSERT_TRUE(subscriber);
}

TEST_F(ConnectionGRPCTests, test_ServerGRPC_startsAndStop)
{
	auto server = _connectionManager->createServer(_config);
	bool startResult = server->start();
	ASSERT_TRUE(startResult);
	// will be stopped by the connection manager
}

TEST_F(ConnectionGRPCTests, test_ClientGRPC_startsAndStop_When_noServer)
{
	auto client = _connectionManager->createClient(_config);
	bool startResult = client->start();
	ASSERT_FALSE(startResult);
	// will be stopped by the connection manager
}

TEST_F(ConnectionGRPCTests, test_PublisherGRPC_startsAndStop)
{
	auto publisher = _connectionManager->createPublisher(_config);
	bool startResult = publisher->start();
	ASSERT_TRUE(startResult);
	// will be stopped by the connection manager
}

TEST_F(ConnectionGRPCTests, test_SubscriberGRPC_startsAndStop_When_noServer)
{
	auto subscriber = _connectionManager->createSubscriber(_config);
	bool startResult = subscriber->start();
	ASSERT_FALSE(startResult);
	// will be stopped by the connection manager
}

TEST_F(ConnectionGRPCTests, test_ServerGRPC_doesNotStart_When_PortIsAlreadyUSed)
{
	auto server = _connectionManager->createServer(_config);
	bool startResult = server->start();
	ASSERT_TRUE(startResult);
	auto server2 = _connectionManager->createServer(_config);
	bool startResult2 = server2->start();
	ASSERT_FALSE(startResult2);
}

/* Client / Server connections */

TEST_F(ConnectionGRPCTests, test_ClientGRPC_connectsToServerGRPC)
{
	createServer(_config);
	startServer();

	startClients(_config, 1);
	waitForClientsHandled();
}

TEST_F(ConnectionGRPCTests, test_ServerGRPC_supportsMultipleClients)
{
	createServer(_config);
	startServer();

	startClients(_config, 5);
	waitForClientsHandled();
}

TEST_F(ConnectionGRPCTests, test_ServerGRPC_allowsConfigurationBeforeClientHandling)
{
	createServer(_config);
	startServer();

	EXPECT_CALL(*_clientHandlerMock, configureClient(_)).Times(1).WillRepeatedly([&](const std::shared_ptr<ghost::Client>& client) {
		auto handler = client->addMessageHandler();
		handler->addHandler<google::protobuf::DoubleValue>(std::bind(&ConnectionGRPCTests::doubleMessageHandler, this, std::placeholders::_1));
		});
	EXPECT_CALL(*_clientHandlerMock, handle(_, _)).Times(1).WillRepeatedly([&](std::shared_ptr<ghost::Client> client, bool& keepClientAlive) {
		keepClientAlive = true;
		return true;
		});

	startClients(_config, 1, false);
	_clients[0]->getWriter<google::protobuf::DoubleValue>()->write(google::protobuf::DoubleValue::default_instance());
	waitForClientsHandled();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	ASSERT_TRUE(_doubleValueMessageWasHandledCounter == 1);
}

TEST_F(ConnectionGRPCTests, test_ServerGRPC_allowsClientsToBeStoredSomewhere)
{
	createServer(_config);
	startServer();

	std::shared_ptr<ghost::Client> remote;

	EXPECT_CALL(*_clientHandlerMock, configureClient(_)).Times(1);
	EXPECT_CALL(*_clientHandlerMock, handle(_, _)).Times(1).WillRepeatedly(testing::DoAll(
		testing::SetArgReferee<1>(true), // keep client = true
		testing::SaveArg<0>(&remote),
		testing::Return(true)));

	startClients(_config, 1, false);
	waitForClientsHandled();
	std::this_thread::sleep_for(std::chrono::milliseconds(10)); // after the client is handled, wait a bit to check that the server didn't close it
	ASSERT_TRUE(remote);
	ASSERT_TRUE(remote->isRunning());
}

TEST_F(ConnectionGRPCTests, test_ServerGRPC_stops_When_clientHandlerReturnsFalse)
{
	createServer(_config);
	startServer();

	std::shared_ptr<ghost::Client> remote;

	EXPECT_CALL(*_clientHandlerMock, configureClient(_)).Times(1);
	EXPECT_CALL(*_clientHandlerMock, handle(_, _)).Times(1).WillOnce(testing::Return(false));

	startClients(_config, 1, false);
	auto now = std::chrono::steady_clock::now();
	auto deadline = now + std::chrono::seconds(1);
	while (now < deadline && _server->isRunning())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		now = std::chrono::steady_clock::now();
	}
	ASSERT_FALSE(_server->isRunning());
}

/* Subscriber / Publisher connections */

TEST_F(ConnectionGRPCTests, test_SubscriberGRPC_connectsToPublisherGRPC)
{
	createPublisher(_config);
	startPublisher();

	int subscribersCount = 1;
	startSubscribers(_config, subscribersCount);
	setupSubscribers(subscribersCount);

	auto writer = _publisher->getWriter<google::protobuf::DoubleValue>();
	bool writeResult = writer->write(google::protobuf::DoubleValue::default_instance());
	ASSERT_TRUE(writeResult);

	checkSubscribersReceivedMessages(subscribersCount);
}

TEST_F(ConnectionGRPCTests, test_PublisherGRPC_supportsMultipleClients)
{
	createPublisher(_config);
	startPublisher();

	int subscribersCount = 10;
	startSubscribers(_config, subscribersCount);
	setupSubscribers(subscribersCount);

	auto writer = _publisher->getWriter<google::protobuf::DoubleValue>();
	bool writeResult = writer->write(google::protobuf::DoubleValue::default_instance());
	ASSERT_TRUE(writeResult);

	checkSubscribersReceivedMessages(subscribersCount);
}

TEST_F(ConnectionGRPCTests, test_PublisherGRPC_continuesOperation_When_subscriberDies)
{
	createPublisher(_config);
	startPublisher();

	int subscribersCount = 2;
	startSubscribers(_config, subscribersCount);
	setupSubscribers(subscribersCount);

	// send the first message, both subscribers should receive it
	auto writer = _publisher->getWriter<google::protobuf::DoubleValue>();
	bool writeResult = writer->write(google::protobuf::DoubleValue::default_instance());
	ASSERT_TRUE(writeResult);

	checkSubscribersReceivedMessages(subscribersCount);

	// stop the last subscriber
	bool stopResult = _subscribers[1]->stop();

	// Reset statistics and send another message
	_doubleValueMessageWasHandledMap.clear();
	writeResult = writer->write(google::protobuf::DoubleValue::default_instance());
	ASSERT_TRUE(writeResult);

	checkSubscribersReceivedMessages(1); // only the first one should receive a message
	ASSERT_FALSE(_subscribers[1]->isRunning());
}

TEST_F(ConnectionGRPCTests, test_ServerGRPC_doesNotHang_When_remoteClientIsAddedWhileStopIsCalled)
{
	createServer(_config);
	startServer();
	startClients(_config, 1, false);
	bool stopResult = _server->stop();
	ASSERT_TRUE(stopResult);
}

/*TEST_F(ConnectionGRPCTests, test_SubscriberGRPC_continuesOperation_When_PublisherDies)
{
	// stop publisher and then restart it

	// That will not work!
}*/
