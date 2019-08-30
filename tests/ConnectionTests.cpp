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

/**
 *	This test class groups the following test categories:
 *	- ConnectionManger
 *	- ConnectionFactory
 */
class ConnectionTests : public testing::Test
{
protected:
	void SetUp() override
	{

	}

	void TearDown() override
	{

	}
};

TEST_F(ConnectionTests, test_ConnectionManager_createServerFails_When_nothingConfigured)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createClientFails_When_nothingConfigured)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createPublisherFails_When_nothingConfigured)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createSubscriberFails_When_nothingConfigured)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createServerSucceeds_When_configurationExists)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createClientSucceeds_When_configurationExists)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createPublisherSucceeds_When_configurationExists)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createSubscriberSucceeds_When_configurationExists)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createSucceeds_When_requiredEmptyConfigurationAttributeIsSet)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createFails_When_requiredNonEmptyConfigurationAttributeIsDifferent)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_createFails_When_requiredConfigurationAttributeIsMissing)
{

}

TEST_F(ConnectionTests, test_ConnectionManager_closesConnection_When_destructorIsCalled)
{

}

//TEST_F(ConnectionTests, test_connection_manager)
//{
//	ghost::ConnectionConfiguration config;
//	config.addAttribute("param", "");
//
//	auto connectionManager = ghost::ConnectionManager::create();
//	auto factory = connectionManager->getConnectionFactory();
//
//	auto client = connectionManager->createClient(config);
//	auto server = connectionManager->createServer(config);
//	auto publisher = connectionManager->createPublisher(config);
//	auto subscriber = connectionManager->createSubscriber(config);
//	ASSERT_TRUE(!client); // no rules -> no response
//	ASSERT_TRUE(!server); // no rules -> no response
//	ASSERT_TRUE(!publisher); // no rules -> no response
//	ASSERT_TRUE(!subscriber); // no rules -> no response
//	
//	factory->addPublisherRule<PublisherMock>(config); // empty
//	auto client2 = connectionManager->createClient(config);
//	auto server2 = connectionManager->createServer(config);
//	auto publisher2 = connectionManager->createPublisher(config);
//	auto subscriber2 = connectionManager->createSubscriber(config);
//	ASSERT_TRUE(!client2); // no rules -> no response
//	ASSERT_TRUE(!server2); // no rules -> no response
//	ASSERT_TRUE(publisher2); // open rule -> will return something
//	ASSERT_TRUE(!subscriber2); // no rules -> no response
//	
//	publisher2->start();
//	ASSERT_TRUE(publisher2->isRunning());
//
//	factory->addServerRule<ServerMock>(config); // empty
//	auto client3 = connectionManager->createClient(config);
//	auto server3 = connectionManager->createServer(config);
//	auto publisher3 = connectionManager->createPublisher(config);
//	auto subscriber3 = connectionManager->createSubscriber(config);
//	ASSERT_TRUE(!client3); // no rules -> no response
//	ASSERT_TRUE(server3); // open rule -> will return something
//	ASSERT_TRUE(publisher3); // open rule -> will return something
//	ASSERT_TRUE(!subscriber3); // no rules -> no response
//
//	server3->start();
//	ASSERT_TRUE(server3->isRunning());
//	publisher3->start();
//	ASSERT_TRUE(publisher3->isRunning());
//
//	config.updateAttribute("param", "john");
//
//	factory->addClientRule<ClientMock>(config); // empty
//	factory->addSubscriberRule<SubscriberMock>(config); // empty
//	auto client4 = connectionManager->createClient(config);
//	auto server4 = connectionManager->createServer(config);
//	auto publisher4 = connectionManager->createPublisher(config);
//	auto subscriber4 = connectionManager->createSubscriber(config);
//	ASSERT_TRUE(client4); // open rule -> will return something
//	ASSERT_TRUE(server4); // open rule -> will return something
//	ASSERT_TRUE(publisher4); // open rule -> will return something
//	ASSERT_TRUE(subscriber4); // open rule -> will return something
//
//	client4->start();
//	ASSERT_TRUE(client4->isRunning());
//	server4->start();
//	ASSERT_TRUE(server4->isRunning());
//	publisher4->start();
//	ASSERT_TRUE(publisher4->isRunning());
//	subscriber4->start();
//	ASSERT_TRUE(subscriber4->isRunning());
//
//	ghost::ConnectionConfiguration config2; // empty configuration
//
//	auto client5 = connectionManager->createClient(config2);
//	auto server5 = connectionManager->createServer(config2);
//	auto publisher5 = connectionManager->createPublisher(config2);
//	auto subscriber5 = connectionManager->createSubscriber(config2);
//	ASSERT_TRUE(!client5); // different rule -> no response
//	ASSERT_TRUE(!server5); // different rule -> no response
//	ASSERT_TRUE(!publisher5); // different rule -> no response
//	ASSERT_TRUE(!subscriber5); // different rule -> no respons
//	
//	connectionManager.reset();
//
//	ASSERT_TRUE(!publisher2->isRunning());
//	ASSERT_TRUE(!server3->isRunning());
//	ASSERT_TRUE(!publisher3->isRunning());
//	ASSERT_TRUE(!client4->isRunning());
//	ASSERT_TRUE(!server4->isRunning());
//	ASSERT_TRUE(!publisher4->isRunning());
//	ASSERT_TRUE(!subscriber4->isRunning());
//}
