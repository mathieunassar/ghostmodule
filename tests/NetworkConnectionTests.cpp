#include <catch.hpp>
#include <iostream>

#include <ghost/connection/internal/network/ServerGRPC.hpp>
#include <ghost/connection/internal/network/PublisherGRPC.hpp>
#include <ghost/connection/internal/network/SubscriberGRPC.hpp>
#include <ghost/connection/internal/network/ClientGRPC.hpp>

#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection/NetworkConnectionConfiguration.hpp>
#include <ghost/connection/ProtobufMessage.hpp>

using namespace ghost;

// problems: config not recognized if default constructor used to setup the factory
// problem: dying / closing client blocks the publisher if call is blocking
TEST_CASE("test_network_connections")
{
	NetworkConnectionConfiguration config;

	NetworkConnectionConfiguration publisherConfig;
	publisherConfig.setServerIpAddress("127.0.0.1");
	publisherConfig.setServerPortNumber(50001);
	publisherConfig.setThreadPoolSize(8);
	publisherConfig.setOperationBlocking(false);

	auto connectionManager = ghost::ConnectionManager::create();
	auto factory = connectionManager->getConnectionFactory();
	factory->addPublisherRule<internal::PublisherGRPC>(publisherConfig);
	factory->addSubscriberRule<internal::SubscriberGRPC>(publisherConfig);
	factory->addServerRule<internal::ServerGRPC>(config);
	factory->addClientRule<internal::ClientGRPC>(config);

	auto publisher = connectionManager->createPublisher(publisherConfig);
	REQUIRE(publisher);

	auto subscriber = connectionManager->createSubscriber(publisherConfig);
	REQUIRE(subscriber);

	auto subscriber2 = connectionManager->createSubscriber(publisherConfig);
	REQUIRE(subscriber2);

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
		std::cout << "received#: " << val << std::endl;
		if (val < counterReceived)
			wrongOrderCount++;
		counterReceived++;
	});

	auto messageHandler2 = reader2->addMessageHandler();
	messageHandler2->addHandler<internal::protobuf::GenericMessageHeader>([&](const internal::protobuf::GenericMessageHeader& msg) {
		int val = msg.timestamp();
		std::cout << "received2#: " << val << std::endl;
		if (val < counterReceived2)
			wrongOrderCount++;
		counterReceived2++;
	});

	publisher->start();
	subscriber->start();
	subscriber2->start();

	REQUIRE(publisher->isRunning());
	REQUIRE(subscriber->isRunning());
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	while (counterSent < 100'000)
	{
		internal::protobuf::GenericMessageHeader msg;
		msg.set_timestamp(counterSent);
		bool writeSuccess = writer->write(msg);
		REQUIRE(writeSuccess);
		counterSent++;
	}

	std::cout << "finished sending" << std::endl;
	REQUIRE(counterSent == 100000);

	auto end = std::chrono::system_clock::now() + std::chrono::seconds(50);
	auto now = std::chrono::system_clock::now();
	while ((counterReceived < 100000 || counterReceived2 < 100000) && now < end)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		now = std::chrono::system_clock::now();
	}

	REQUIRE(counterReceived == 100000);
	REQUIRE(counterReceived2 == 100000);
	REQUIRE(wrongOrderCount == 0);

	std::cout << "stopping second subscriber" << std::endl;
	subscriber2->stop();

	std::cout << "sending 500 more messages" << std::endl;
	while (counterSent < 150000)
	{
		internal::protobuf::GenericMessageHeader msg;
		msg.set_timestamp(counterSent);
		bool writeSuccess = writer->write(msg);
		REQUIRE(writeSuccess);
		counterSent++;
	}

	auto end2 = std::chrono::system_clock::now() + std::chrono::seconds(50);
	auto now2 = std::chrono::system_clock::now();
	while (counterReceived < 150000 && now2 < end2)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		now2 = std::chrono::system_clock::now();
	}

	REQUIRE(counterSent == 150000);
	REQUIRE(counterReceived == 150000); // important is that it kept receiving messages
	REQUIRE(counterReceived2 == 100000);

	publisher->stop();
	subscriber->stop();

	REQUIRE(!publisher->isRunning());
	REQUIRE(!subscriber->isRunning());
}
