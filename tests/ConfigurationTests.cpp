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

#include <ghost/connection/ConnectionConfiguration.hpp>

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
};

TEST_F(ConfigurationTests, test_configuration)
{
	ConnectionConfiguration config("super");

	// Test: add element
	bool addSuccess0 = config.addAttribute("attribute1", 57);
	ASSERT_TRUE(addSuccess0);

	// Test: standard get function
	int val;
	bool getSuccess = config.getAttribute("attribute1", val);
	ASSERT_TRUE(getSuccess);
	ASSERT_TRUE(val == 57);
	ASSERT_TRUE(config.hasAttribute("attribute1"));

	// Test: add without override existing parameter
	bool addSuccess = config.addAttribute("attribute1", "super");
	ASSERT_TRUE(!addSuccess);

	// Test: add and override existing parameter
	bool addSuccess2 = config.addAttribute("attribute1", "super2", true);
	ASSERT_TRUE(addSuccess2);
	std::string result;
	bool getSuccess4 = config.getAttribute("attribute1", result);
	ASSERT_TRUE(getSuccess4);
	ASSERT_TRUE(result == "super2");

	// Test: update existing parameter
	bool updateSuccess = config.updateAttribute("attribute1", "fini");
	ASSERT_TRUE(updateSuccess);
	std::string result2;
	bool getSuccess5 = config.getAttribute("attribute1", result2);
	ASSERT_TRUE(getSuccess5);
	ASSERT_TRUE(result2 == "fini");

	// Test: update non-existent parameter
	bool updateSuccess2 = config.updateAttribute("yo", "test");
	ASSERT_TRUE(!updateSuccess2);

	// Test: removal and removal of non existing parameter
	bool removeSuccess = config.removeAttribute("attribute1");
	ASSERT_TRUE(removeSuccess);
	bool removeSuccess2 = config.removeAttribute("attribute1");
	ASSERT_TRUE(!removeSuccess2);

	// Test: get configuration name
	ASSERT_TRUE(config.getConfigurationName() == "super");

	// Test: get element after deletion
	int sup = 4;
	bool getSuccess2 = config.getAttribute("attribute1", sup);
	ASSERT_TRUE(!getSuccess2);
	ASSERT_TRUE(sup == 4);

	// Test: wrong conversion
	config.addAttribute("strAttribute", "mystring");
	int intValue = 42;
	bool getSuccess3 = config.getAttribute("strAttribute", intValue);
	ASSERT_TRUE(!getSuccess3);
	//ASSERT_TRUE(intValue == 42); // if the conversion fails, there is no guarantee on the result

	// Test: empty configuration field
	config.addAttribute("empty", "");
	bool hasSuccess = config.hasAttribute("empty");
	bool emptySuccess = config.isAttributeEmpty("empty");
	ASSERT_TRUE(hasSuccess);
	ASSERT_TRUE(emptySuccess);
}

TEST_F(ConfigurationTests, test_connectionConfiguration)
{
	ConnectionConfiguration config;

	ASSERT_TRUE(config.getThreadPoolSize() == 2);

	config.removeAttribute("CONNECTIONCONFIGURATION_ID");

	ASSERT_TRUE(config.getConnectionId() == -1);
	
	config.setConnectionId(45);

	ASSERT_TRUE(config.getConnectionId() == 45);

	config.setOperationBlocking(false);
	ASSERT_TRUE(!config.isOperationBlocking());

	config.setOperationBlocking(true);
	ASSERT_TRUE(config.isOperationBlocking());

	ConnectionConfiguration copied;
	config.copy(copied);

	ASSERT_TRUE(config.isOperationBlocking());
	ASSERT_TRUE(config.getConnectionId() == 45);
}
