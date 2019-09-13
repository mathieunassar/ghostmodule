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

#include <ghost/connection/Configuration.hpp>
#include <ghost/connection/ConnectionConfiguration.hpp>
#include <ghost/connection/NetworkConnectionConfiguration.hpp>

using namespace ghost;

class ConfigurationTests : public testing::Test
{
protected:
	void SetUp() override
	{

	}

	void TearDown() override
	{

	}

	static const std::string TEST_CONFIGURATION_NAME;
	static const std::string TEST_CONFIGURATION_FIELD;
	static const std::string TEST_CONFIGURATION_VALUE;
	static const std::string TEST_CONFIGURATION_VALUE2;
	static const int TEST_CONFIGURATION_VALUE_INT;
};

const std::string ConfigurationTests::TEST_CONFIGURATION_NAME = "TEST_NAME";
const std::string ConfigurationTests::TEST_CONFIGURATION_FIELD = "TEST_FIELD";
const std::string ConfigurationTests::TEST_CONFIGURATION_VALUE = "TEST_VALUE";
const std::string ConfigurationTests::TEST_CONFIGURATION_VALUE2 = "TEST_VALUE2";
const int ConfigurationTests::TEST_CONFIGURATION_VALUE_INT = 42;


TEST_F(ConfigurationTests, test_configuration_addElement)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	ASSERT_FALSE(configuration->hasAttribute(TEST_CONFIGURATION_FIELD));

	bool addResult = configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));

	ASSERT_TRUE(addResult);
	ASSERT_TRUE(configuration->hasAttribute(TEST_CONFIGURATION_FIELD));
	ASSERT_TRUE(configuration->getAttributes().size() == 1);
}

TEST_F(ConfigurationTests, test_configuration_addElement_When_alreadyExistsAndNoOverwrite)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));

	bool addResult = configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));
	
	ASSERT_FALSE(addResult);
	ASSERT_TRUE(configuration->hasAttribute(TEST_CONFIGURATION_FIELD));
	ASSERT_TRUE(configuration->getAttributes().size() == 1);
}

TEST_F(ConfigurationTests, test_configuration_addElement_When_alreadyExistsAndOverwrite)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));

	bool addResult = configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE), true);

	ASSERT_TRUE(addResult);
	ASSERT_TRUE(configuration->hasAttribute(TEST_CONFIGURATION_FIELD));
	ASSERT_TRUE(configuration->getAttributes().size() == 1);
}

TEST_F(ConfigurationTests, test_configuration_getElement)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));

	ghost::ConfigurationValue value;
	bool getResult = configuration->getAttribute(TEST_CONFIGURATION_FIELD, value);

	ASSERT_TRUE(getResult);

	std::string myValue;
	bool readResult = value.read(myValue);
	ASSERT_TRUE(readResult);
	ASSERT_TRUE(myValue == TEST_CONFIGURATION_VALUE);
}

TEST_F(ConfigurationTests, test_configuration_getElement_When_elementDoesNotExist)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	
	ghost::ConfigurationValue value;
	bool getResult = configuration->getAttribute(TEST_CONFIGURATION_FIELD, value);

	ASSERT_FALSE(getResult);
}

TEST_F(ConfigurationTests, test_configuration_update)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));

	bool updateResult = configuration->updateAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE2));
	ASSERT_TRUE(updateResult);
	
	ghost::ConfigurationValue value;
	configuration->getAttribute(TEST_CONFIGURATION_FIELD, value);

	std::string myValue;
	bool readResult = value.read(myValue);
	ASSERT_TRUE(myValue == TEST_CONFIGURATION_VALUE2);
}

TEST_F(ConfigurationTests, test_configuration_update_When_elementDoesNotExist)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);

	bool updateResult = configuration->updateAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE2));
	ASSERT_FALSE(updateResult);

	ASSERT_TRUE(configuration->getAttributes().size() == 0);
}

TEST_F(ConfigurationTests, test_configuration_remove)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));

	bool removeResult = configuration->removeAttribute(TEST_CONFIGURATION_FIELD);
	ASSERT_TRUE(removeResult);
	ASSERT_TRUE(configuration->getAttributes().size() == 0);
	ASSERT_FALSE(configuration->hasAttribute(TEST_CONFIGURATION_FIELD));
}

TEST_F(ConfigurationTests, test_configuration_remove_When_elementDoesNotExist)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);

	bool removeResult = configuration->removeAttribute(TEST_CONFIGURATION_FIELD);
	ASSERT_FALSE(removeResult);
	ASSERT_TRUE(configuration->getAttributes().size() == 0);
	ASSERT_FALSE(configuration->hasAttribute(TEST_CONFIGURATION_FIELD));
}

TEST_F(ConfigurationTests, test_configuration_getConfigurationName)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	ASSERT_TRUE(configuration->getConfigurationName() == TEST_CONFIGURATION_NAME);
}

TEST_F(ConfigurationTests, test_configuration_get_When_elementWasRemoved)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));
	configuration->removeAttribute(TEST_CONFIGURATION_FIELD);

	ghost::ConfigurationValue value;
	bool getResult = configuration->getAttribute(TEST_CONFIGURATION_FIELD, value);

	ASSERT_FALSE(getResult);
}

TEST_F(ConfigurationTests, test_configuration_get_When_wrongConversion)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));

	ghost::ConfigurationValue value;
	configuration->getAttribute(TEST_CONFIGURATION_FIELD, value);

	int myValue;
	bool readResult = value.read(myValue);
	ASSERT_FALSE(readResult);
}

TEST_F(ConfigurationTests, test_configuration_emptyField)
{
	auto configuration = ghost::Configuration::create(TEST_CONFIGURATION_NAME);
	configuration->addAttribute(TEST_CONFIGURATION_FIELD, ghost::Configuration::EMPTY);

	ASSERT_TRUE(configuration->hasAttribute(TEST_CONFIGURATION_FIELD));
	ASSERT_TRUE(configuration->isAttributeEmpty(TEST_CONFIGURATION_FIELD));
}

TEST_F(ConfigurationTests, test_connectionConfiguration_blocking)
{
	ghost::ConnectionConfiguration configuration;
	ASSERT_TRUE(configuration.isOperationBlocking());
	configuration.setOperationBlocking(false);
	ASSERT_FALSE(configuration.isOperationBlocking());
}

TEST_F(ConfigurationTests, test_connectionConfiguration_connectionId)
{
	ghost::ConnectionConfiguration configuration;
	ASSERT_TRUE(configuration.getConnectionId() == -1);
	configuration.setConnectionId(TEST_CONFIGURATION_VALUE_INT);
	ASSERT_TRUE(configuration.getConnectionId() == TEST_CONFIGURATION_VALUE_INT);
}

TEST_F(ConfigurationTests, test_connectionConfiguration_threadPoolSize)
{
	ghost::ConnectionConfiguration configuration;
	ASSERT_TRUE(configuration.getThreadPoolSize() == 2);
	configuration.setThreadPoolSize(TEST_CONFIGURATION_VALUE_INT);
	ASSERT_TRUE(configuration.getThreadPoolSize() == TEST_CONFIGURATION_VALUE_INT);
}

TEST_F(ConfigurationTests, test_connectionConfiguration_configurationIsUpdateable)
{
	ghost::ConnectionConfiguration configuration;
	ASSERT_TRUE(configuration.getConfiguration()->getAttributes().size() == 3);
	configuration.getConfiguration()->addAttribute(TEST_CONFIGURATION_FIELD, ghost::ConfigurationValue(TEST_CONFIGURATION_VALUE));
	ASSERT_TRUE(configuration.getConfiguration()->hasAttribute(TEST_CONFIGURATION_FIELD));
}

TEST_F(ConfigurationTests, test_networkconnectionConfiguration_ip)
{
	ghost::NetworkConnectionConfiguration configuration;
	ASSERT_TRUE(configuration.getServerIpAddress() == "127.0.0.1");
	configuration.setServerIpAddress(TEST_CONFIGURATION_VALUE);
	ASSERT_TRUE(configuration.getServerIpAddress() == TEST_CONFIGURATION_VALUE);
}

TEST_F(ConfigurationTests, test_networkconnectionConfiguration_port)
{
	ghost::NetworkConnectionConfiguration configuration;
	ASSERT_TRUE(configuration.getServerPortNumber() == -1);
	configuration.setServerPortNumber(TEST_CONFIGURATION_VALUE_INT);
	ASSERT_TRUE(configuration.getServerPortNumber() == TEST_CONFIGURATION_VALUE_INT);
}
