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

TEST_CASE("test_network_connections")
{
	NetworkConnectionConfiguration config;

	auto connectionManager = ghost::ConnectionManager::create();
	auto factory = connectionManager->getConnectionFactory();
	factory->addPublisherRule<internal::PublisherGRPC>(config);
	factory->addSubscriberRule<internal::SubscriberGRPC>(config);
	factory->addServerRule<internal::ServerGRPC>(config);
	factory->addClientRule<internal::ClientGRPC>(config);

	NetworkConnectionConfiguration publisherConfig;
	config.setServerIpAddress("127.0.0.1");
	config.setServerPortNumber(50001);
	config.setThreadPoolSize(8);
	config.setOperationBlocking(false);

	auto publisher = connectionManager->createPublisher(publisherConfig);
	REQUIRE(publisher);

	auto subscriber = connectionManager->createSubscriber(publisherConfig);
	REQUIRE(subscriber);

	auto writer = publisher->getWriter<internal::protobuf::GenericMessageHeader>();
	auto reader = subscriber->getReader<internal::protobuf::GenericMessageHeader>();
	int counterSent = 0;
	int counterReceived = 0;
	int wrongOrderCount = 0;

	auto messageHandler = reader->addMessageHandler();
	messageHandler->addHandler<internal::protobuf::GenericMessageHeader>([&](const internal::protobuf::GenericMessageHeader& msg) {
		int val = msg.timestamp();
		std::cout << "received#: " << val << std::endl;
		if (val < counterReceived)
			wrongOrderCount++;
		counterReceived++;
	});

	publisher->start();
	subscriber->start();

	REQUIRE(publisher->isRunning());
	REQUIRE(subscriber->isRunning());
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	while (counterSent < 1000)
	{
		internal::protobuf::GenericMessageHeader msg;
		msg.set_timestamp(counterSent);
		bool writeSuccess = writer->write(msg);
		REQUIRE(writeSuccess);
		counterSent++;
	}

	std::cout << "finished sending" << std::endl;
	REQUIRE(counterSent == 1000);

	auto end = std::chrono::system_clock::now() + std::chrono::seconds(1);
	auto now = std::chrono::system_clock::now();
	while (counterReceived < 1000 && now < end)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		now = std::chrono::system_clock::now();
	}

	REQUIRE(counterReceived == 1000);
	REQUIRE(wrongOrderCount == 0);

	publisher->stop();
	subscriber->stop();

	REQUIRE(!publisher->isRunning());
	REQUIRE(!subscriber->isRunning());
}
