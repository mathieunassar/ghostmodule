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
#include "ConnectionTestUtils.hpp"

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
		_connectionManager = ghost::ConnectionManager::create();
	}

	void TearDown() override
	{

	}

	void registerMockFactories(const ghost::ConnectionConfiguration minimumConfiguration)
	{
		_connectionManager->getConnectionFactory()->addServerRule<ServerMock>(minimumConfiguration);
		_connectionManager->getConnectionFactory()->addClientRule<ClientMock>(minimumConfiguration);
		_connectionManager->getConnectionFactory()->addPublisherRule<PublisherMock>(minimumConfiguration);
		_connectionManager->getConnectionFactory()->addSubscriberRule<SubscriberMock>(minimumConfiguration);
	}

	ghost::ConnectionConfiguration _connectionConfiguration;
	std::shared_ptr<ghost::ConnectionManager> _connectionManager;

	static const int TEST_CONNETION_ID;
	static const std::string TEST_CONFIGURATION_ELEMENT;
};

const int ConnectionTests::TEST_CONNETION_ID = 42;
const std::string ConnectionTests::TEST_CONFIGURATION_ELEMENT = "element";

TEST_F(ConnectionTests, test_ConnectionManager_createServerFails_When_nothingConfigured)
{
	auto server = _connectionManager->createServer(_connectionConfiguration);
	ASSERT_FALSE(server);
}

TEST_F(ConnectionTests, test_ConnectionManager_createClientFails_When_nothingConfigured)
{
	auto client = _connectionManager->createClient(_connectionConfiguration);
	ASSERT_FALSE(client);
}

TEST_F(ConnectionTests, test_ConnectionManager_createPublisherFails_When_nothingConfigured)
{
	auto publisher = _connectionManager->createPublisher(_connectionConfiguration);
	ASSERT_FALSE(publisher);
}

TEST_F(ConnectionTests, test_ConnectionManager_createSubscriberFails_When_nothingConfigured)
{
	auto subscriber = _connectionManager->createSubscriber(_connectionConfiguration);
	ASSERT_FALSE(subscriber);
}

TEST_F(ConnectionTests, test_ConnectionManager_createServerSucceeds_When_configurationExists)
{
	registerMockFactories(_connectionConfiguration);
	auto server = _connectionManager->createServer(_connectionConfiguration);
	ASSERT_TRUE(server);
}

TEST_F(ConnectionTests, test_ConnectionManager_createClientSucceeds_When_configurationExists)
{
	registerMockFactories(_connectionConfiguration);
	auto client = _connectionManager->createClient(_connectionConfiguration);
	ASSERT_TRUE(client);
}

TEST_F(ConnectionTests, test_ConnectionManager_createPublisherSucceeds_When_configurationExists)
{
	registerMockFactories(_connectionConfiguration);
	auto publisher = _connectionManager->createPublisher(_connectionConfiguration);
	ASSERT_TRUE(publisher);
}

TEST_F(ConnectionTests, test_ConnectionManager_createSubscriberSucceeds_When_configurationExists)
{
	registerMockFactories(_connectionConfiguration);
	auto subscriber = _connectionManager->createSubscriber(_connectionConfiguration);
	ASSERT_TRUE(subscriber);
}

TEST_F(ConnectionTests, test_ConnectionManager_createSucceeds_When_requiredEmptyConfigurationAttributeIsSet)
{
	registerMockFactories(_connectionConfiguration);
	ghost::ConnectionConfiguration config;
	config.setConnectionId(TEST_CONNETION_ID);
	auto server = _connectionManager->createServer(config);
	ASSERT_TRUE(server);
}

TEST_F(ConnectionTests, test_ConnectionManager_createFails_When_requiredNonEmptyConfigurationAttributeIsDifferent)
{
	_connectionConfiguration.setConnectionId(TEST_CONNETION_ID);
	registerMockFactories(_connectionConfiguration);
	ghost::ConnectionConfiguration config;
	auto server = _connectionManager->createServer(config);
	ASSERT_FALSE(server);
}

TEST_F(ConnectionTests, test_ConnectionManager_createFails_When_requiredConfigurationAttributeIsMissing)
{
	ghost::ConfigurationValue v;
	v.write<int>(TEST_CONNETION_ID);
	_connectionConfiguration.getConfiguration()->addAttribute(TEST_CONFIGURATION_ELEMENT, v);
	registerMockFactories(_connectionConfiguration);
	ghost::ConnectionConfiguration config;
	auto server = _connectionManager->createServer(config);
	ASSERT_FALSE(server);
}

TEST_F(ConnectionTests, test_ConnectionManager_closesConnection_When_destructorIsCalled)
{
	registerMockFactories(_connectionConfiguration);
	auto server = _connectionManager->createServer(_connectionConfiguration);
	ASSERT_TRUE(server);

	auto cast = std::dynamic_pointer_cast<ServerMock>(server);
	EXPECT_CALL(*cast, stop).Times(1);

	_connectionManager.reset();
}
