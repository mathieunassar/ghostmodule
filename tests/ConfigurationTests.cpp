#include <catch.hpp>
#include <iostream>

#include <ConnectionConfiguration.hpp>

using namespace ghost;

TEST_CASE("test_configuration")
{
	ConnectionConfiguration config("super");

	// Test: add element
	bool addSuccess0 = config.addAttribute("attribute1", 57);
	REQUIRE(addSuccess0);

	// Test: standard get function
	int val;
	bool getSuccess = config.getAttribute("attribute1", val);
	REQUIRE(getSuccess);
	REQUIRE(val == 57);
	REQUIRE(config.hasAttribute("attribute1"));

	// Test: add without override existing parameter
	bool addSuccess = config.addAttribute("attribute1", "super");
	REQUIRE(!addSuccess);

	// Test: add and override existing parameter
	bool addSuccess2 = config.addAttribute("attribute1", "super2", true);
	REQUIRE(addSuccess2);
	std::string result;
	bool getSuccess4 = config.getAttribute("attribute1", result);
	REQUIRE(getSuccess4);
	REQUIRE(result == "super2");

	// Test: update existing parameter
	bool updateSuccess = config.updateAttribute("attribute1", "fini");
	REQUIRE(updateSuccess);
	std::string result2;
	bool getSuccess5 = config.getAttribute("attribute1", result2);
	REQUIRE(getSuccess5);
	REQUIRE(result2 == "fini");

	// Test: update non-existent parameter
	bool updateSuccess2 = config.updateAttribute("yo", "test");
	REQUIRE(!updateSuccess2);

	// Test: removal and removal of non existing parameter
	bool removeSuccess = config.removeAttribute("attribute1");
	REQUIRE(removeSuccess);
	bool removeSuccess2 = config.removeAttribute("attribute1");
	REQUIRE(!removeSuccess2);

	// Test: get configuration name
	REQUIRE(config.getConfigurationName() == "super");

	// Test: get element after deletion
	int sup = 4;
	bool getSuccess2 = config.getAttribute("attribute1", sup);
	REQUIRE(!getSuccess2);
	REQUIRE(sup == 4);

	// Test: wrong conversion
	config.addAttribute("strAttribute", "mystring");
	int intValue = 42;
	bool getSuccess3 = config.getAttribute("strAttribute", intValue);
	REQUIRE(!getSuccess3);
	//REQUIRE(intValue == 42); // if the conversion fails, there is no guarantee on the result
}

TEST_CASE("test_connectionConfiguration")
{
	ConnectionConfiguration config;

	config.removeAttribute("CONNECTIONCONFIGURATION_ID");

	REQUIRE(config.getConnectionId() == -1);
	
	config.setConnectionId(45);

	REQUIRE(config.getConnectionId() == 45);

	config.setOperationBlocking(false);
	REQUIRE(!config.isOperationBlocking());

	config.setOperationBlocking(true);
	REQUIRE(config.isOperationBlocking());

	ConnectionConfiguration copied;
	config.copy(copied);

	REQUIRE(config.isOperationBlocking());
	REQUIRE(config.getConnectionId() == 45);
}
