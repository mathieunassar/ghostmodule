#include <catch.hpp>
#include <iostream>

#include <ghost/connection/ConnectionManager.hpp>
#include "ConnectionMocks.hpp"

using namespace ghost;

TEST_CASE("test_connection_manager")
{
	ghost::ConnectionConfiguration config;
	config.addAttribute("param", "");

	auto connectionManager = ghost::ConnectionManager::create();
	auto factory = connectionManager->getConnectionFactory();

	auto client = connectionManager->createClient(config);
	auto server = connectionManager->createServer(config);
	auto publisher = connectionManager->createPublisher(config);
	auto subscriber = connectionManager->createSubscriber(config);
	REQUIRE(!client); // no rules -> no response
	REQUIRE(!server); // no rules -> no response
	REQUIRE(!publisher); // no rules -> no response
	REQUIRE(!subscriber); // no rules -> no response
	
	factory->addPublisherRule<PublisherMock>(config); // empty
	auto client2 = connectionManager->createClient(config);
	auto server2 = connectionManager->createServer(config);
	auto publisher2 = connectionManager->createPublisher(config);
	auto subscriber2 = connectionManager->createSubscriber(config);
	REQUIRE(!client2); // no rules -> no response
	REQUIRE(!server2); // no rules -> no response
	REQUIRE(publisher2); // open rule -> will return something
	REQUIRE(!subscriber2); // no rules -> no response
	
	publisher2->start();
	REQUIRE(publisher2->isRunning());

	factory->addServerRule<ServerMock>(config); // empty
	auto client3 = connectionManager->createClient(config);
	auto server3 = connectionManager->createServer(config);
	auto publisher3 = connectionManager->createPublisher(config);
	auto subscriber3 = connectionManager->createSubscriber(config);
	REQUIRE(!client3); // no rules -> no response
	REQUIRE(server3); // open rule -> will return something
	REQUIRE(publisher3); // open rule -> will return something
	REQUIRE(!subscriber3); // no rules -> no response

	server3->start();
	REQUIRE(server3->isRunning());
	publisher3->start();
	REQUIRE(publisher3->isRunning());

	config.updateAttribute("param", "john");

	factory->addClientRule<ClientMock>(config); // empty
	factory->addSubscriberRule<SubscriberMock>(config); // empty
	auto client4 = connectionManager->createClient(config);
	auto server4 = connectionManager->createServer(config);
	auto publisher4 = connectionManager->createPublisher(config);
	auto subscriber4 = connectionManager->createSubscriber(config);
	REQUIRE(client4); // open rule -> will return something
	REQUIRE(server4); // open rule -> will return something
	REQUIRE(publisher4); // open rule -> will return something
	REQUIRE(subscriber4); // open rule -> will return something

	client4->start();
	REQUIRE(client4->isRunning());
	server4->start();
	REQUIRE(server4->isRunning());
	publisher4->start();
	REQUIRE(publisher4->isRunning());
	subscriber4->start();
	REQUIRE(subscriber4->isRunning());

	ghost::ConnectionConfiguration config2; // empty configuration

	auto client5 = connectionManager->createClient(config2);
	auto server5 = connectionManager->createServer(config2);
	auto publisher5 = connectionManager->createPublisher(config2);
	auto subscriber5 = connectionManager->createSubscriber(config2);
	REQUIRE(!client5); // different rule -> no response
	REQUIRE(!server5); // different rule -> no response
	REQUIRE(!publisher5); // different rule -> no response
	REQUIRE(!subscriber5); // different rule -> no respons
	
	connectionManager.reset();

	REQUIRE(!publisher2->isRunning());
	REQUIRE(!server3->isRunning());
	REQUIRE(!publisher3->isRunning());
	REQUIRE(!client4->isRunning());
	REQUIRE(!server4->isRunning());
	REQUIRE(!publisher4->isRunning());
	REQUIRE(!subscriber4->isRunning());
}
