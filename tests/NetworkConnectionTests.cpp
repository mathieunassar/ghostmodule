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
#include <gtest/gtest.h>

#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection/NetworkConnectionConfiguration.hpp>
#include <ghost/connection/internal/ProtobufMessage.hpp>

using namespace ghost;

class NetworkConnectionTests : public testing::Test
{
protected:
	void SetUp() override
	{

	}

	void TearDown() override
	{

	}
};

TEST_F(NetworkConnectionTests, test_network_connections)
{
	NetworkConnectionConfiguration publisherConfig;
	publisherConfig.setServerIpAddress("127.0.0.1");
	publisherConfig.setServerPortNumber(50001);
	publisherConfig.setThreadPoolSize(8);
	publisherConfig.setOperationBlocking(false);
	publisherConfig.removeAttribute("CONNECTIONCONFIGURATION_BLOCKING");

	auto connectionManager = ghost::ConnectionManager::create();

	auto badpublisher = connectionManager->createPublisher(publisherConfig);
	ASSERT_TRUE(!badpublisher); // fails because the blocking attribute was removed from the configuration

	publisherConfig.addAttribute("CONNECTIONCONFIGURATION_BLOCKING", false);

	auto publisher = connectionManager->createPublisher(publisherConfig);
	ASSERT_TRUE(publisher); // succeeds because the blocking attribute was added again

	auto subscriber = connectionManager->createSubscriber(publisherConfig);
	ASSERT_TRUE(subscriber);

	auto subscriber2 = connectionManager->createSubscriber(publisherConfig);
	ASSERT_TRUE(subscriber2);

	auto writer = publisher->getWriter<internal::protobuf::GenericMessageHeader>();
	auto reader = subscriber->getReader<internal::protobuf::GenericMessageHeader>();
	auto reader2 = subscriber2->getReader<internal::protobuf::GenericMessageHeader>();
	int counterSent = 0;
	int counterReceived = 0;
	int counterReceived2 = 0;
	int wrongOrderCount = 0;

	auto messageHandler = reader->addMessageHandler();
	messageHandler->addHandler<internal::protobuf::GenericMessageHeader>([&](const internal::protobuf::GenericMessageHeader& msg) {
		int val = msg.timestamp();
		//std::cout << "received#: " << val << std::endl;
		if (val < counterReceived)
			wrongOrderCount++;
		counterReceived++;
	});

	auto messageHandler2 = reader2->addMessageHandler();
	messageHandler2->addHandler<internal::protobuf::GenericMessageHeader>([&](const internal::protobuf::GenericMessageHeader& msg) {
		int val = msg.timestamp();
		//std::cout << "received2#: " << val << std::endl;
		if (val < counterReceived2)
			wrongOrderCount++;
		counterReceived2++;
	});

	publisher->start();
	subscriber->start();
	subscriber2->start();

	ASSERT_TRUE(publisher->isRunning());
	ASSERT_TRUE(subscriber->isRunning());
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	while (counterSent < 100'000)
	{
		internal::protobuf::GenericMessageHeader msg;
		msg.set_timestamp(counterSent);
		bool writeSuccess = writer->write(msg);
		ASSERT_TRUE(writeSuccess);
		counterSent++;
	}

	std::cout << "finished sending" << std::endl;
	ASSERT_TRUE(counterSent == 100000);

	auto end = std::chrono::system_clock::now() + std::chrono::seconds(50);
	auto now = std::chrono::system_clock::now();
	while ((counterReceived < 100000 || counterReceived2 < 100000) && now < end)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		now = std::chrono::system_clock::now();
	}

	ASSERT_TRUE(counterReceived == 100000);
	ASSERT_TRUE(counterReceived2 == 100000);
	ASSERT_TRUE(wrongOrderCount == 0);

	std::cout << "stopping second subscriber" << std::endl;
	subscriber2->stop();

	std::cout << "sending 500 more messages" << std::endl;
	while (counterSent < 150000)
	{
		internal::protobuf::GenericMessageHeader msg;
		msg.set_timestamp(counterSent);
		bool writeSuccess = writer->write(msg);
		ASSERT_TRUE(writeSuccess);
		counterSent++;
	}

	auto end2 = std::chrono::system_clock::now() + std::chrono::seconds(50);
	auto now2 = std::chrono::system_clock::now();
	while (counterReceived < 150000 && now2 < end2)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		now2 = std::chrono::system_clock::now();
	}

	ASSERT_TRUE(counterSent == 150000);
	ASSERT_TRUE(counterReceived == 150000); // important is that it kept receiving messages
	ASSERT_TRUE(counterReceived2 == 100000);

	publisher->stop();
	subscriber->stop();

	ASSERT_TRUE(!publisher->isRunning());
	ASSERT_TRUE(!subscriber->isRunning());
}
