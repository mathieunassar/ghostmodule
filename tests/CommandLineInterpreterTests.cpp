/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../src/module/CommandLineParser.hpp"

#include <gtest/gtest.h>
#include <ghost/module/StdoutLogger.hpp>
#include <ghost/module/Command.hpp>
#include <ghost/module/CommandLineInterpreter.hpp>
#include <ghost/module/UserManager.hpp>
#include "../src/module/User.hpp"

class CommandLineInterpreterTest : public testing::Test
{
protected:
	void SetUp() override
	{
		
	}

	void TearDown() override
	{

	}

	static const std::string TEST_COMMAND_LINE_CMDNAME;

	static const std::string TEST_COMMAND_LINE_PARAMA;
	static const std::string TEST_COMMAND_LINE_PARAMB;
	static const std::string TEST_COMMAND_LINE_PARAMC;
	static const std::string TEST_COMMAND_LINE_STRING;

	static const std::string TEST_COMMAND_LINE_NAMEA;
	static const std::string TEST_COMMAND_LINE_NAMEB;
	static const std::string TEST_COMMAND_LINE_STRING_WITH_PARAMNAME;

	static const std::string TEST_COMMAND_LINE_NAMEILLEGAL;
	static const std::string TEST_COMMAND_LINE_STRING_WITH_ILLEGAL;

	static const std::string TEST_COMMAND_LINE_STRING_WITH_MIXED;
};

class CustomCommand : public ghost::Command
{
public:
	CustomCommand(bool returnResult)
		: _returnResult(returnResult)
		, _executeWasCalled(false)
	{

	}

	bool execute(const ghost::CommandLine& commandLine) override
	{
		_executeWasCalled = true;
		return _returnResult;
	}

	std::string getName() const override
	{
		return "name";
	}

	std::string getShortcut() const override
	{
		return "shortcut";
	}

	std::string getDescription() const override
	{
		return "description";
	}

	bool _returnResult;
	bool _executeWasCalled;
};

const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_CMDNAME = "testCommand";

const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_PARAMA = "paramA";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_PARAMB = "paramB";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_PARAMC = "paramC";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_STRING = TEST_COMMAND_LINE_CMDNAME + " " + TEST_COMMAND_LINE_PARAMA + " " + TEST_COMMAND_LINE_PARAMB + " " + TEST_COMMAND_LINE_PARAMC;


const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_NAMEA = "nameA";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_NAMEB = "nameB";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_STRING_WITH_PARAMNAME = TEST_COMMAND_LINE_CMDNAME + " --" + TEST_COMMAND_LINE_NAMEA + " " + TEST_COMMAND_LINE_PARAMA + " --" + TEST_COMMAND_LINE_NAMEB + " " + TEST_COMMAND_LINE_PARAMB;
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_STRING_WITH_MIXED = TEST_COMMAND_LINE_CMDNAME + " " + TEST_COMMAND_LINE_PARAMC + " --" + TEST_COMMAND_LINE_NAMEA + " " + TEST_COMMAND_LINE_PARAMA + " " + TEST_COMMAND_LINE_PARAMB;


const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_NAMEILLEGAL = "__0";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_STRING_WITH_ILLEGAL = TEST_COMMAND_LINE_CMDNAME + " " + TEST_COMMAND_LINE_PARAMC + " --" + TEST_COMMAND_LINE_NAMEILLEGAL + " " + TEST_COMMAND_LINE_PARAMA;

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_emptyLine)
{
	ghost::internal::CommandLineParser parser;
	
	bool exceptionCaught = false;
	try
	{
		parser.parseCommandLine("");
	}
	catch (std::exception e)
	{
		exceptionCaught = true;
	}
	ASSERT_TRUE(exceptionCaught);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_unknownParameters)
{
	ghost::internal::CommandLineParser parser;
	ghost::CommandLine line = parser.parseCommandLine(TEST_COMMAND_LINE_STRING);
	ASSERT_TRUE(line.getCommandName() == TEST_COMMAND_LINE_CMDNAME);
	ASSERT_TRUE(line.getParametersMap().size() == 3);
	ASSERT_TRUE(line.hasParameter("__0"));
	ASSERT_TRUE(line.hasParameter("__1"));
	ASSERT_TRUE(line.hasParameter("__2"));
	ASSERT_TRUE(line.getParameter<std::string>("__0") == TEST_COMMAND_LINE_PARAMA);
	ASSERT_TRUE(line.getParameter<std::string>("__1") == TEST_COMMAND_LINE_PARAMB);
	ASSERT_TRUE(line.getParameter<std::string>("__2") == TEST_COMMAND_LINE_PARAMC);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_noParameters)
{
	ghost::internal::CommandLineParser parser;
	ghost::CommandLine line = parser.parseCommandLine(TEST_COMMAND_LINE_CMDNAME);
	ASSERT_TRUE(line.getCommandName() == TEST_COMMAND_LINE_CMDNAME);
	ASSERT_TRUE(line.getParametersMap().size() == 0);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_namedParameters)
{
	ghost::internal::CommandLineParser parser;
	ghost::CommandLine line = parser.parseCommandLine(TEST_COMMAND_LINE_STRING_WITH_PARAMNAME);
	ASSERT_TRUE(line.getCommandName() == TEST_COMMAND_LINE_CMDNAME);
	ASSERT_TRUE(line.getParametersMap().size() == 2);
	ASSERT_TRUE(line.hasParameter(TEST_COMMAND_LINE_NAMEA));
	ASSERT_TRUE(line.hasParameter(TEST_COMMAND_LINE_NAMEB));
	ASSERT_TRUE(line.getParameter<std::string>(TEST_COMMAND_LINE_NAMEA) == TEST_COMMAND_LINE_PARAMA);
	ASSERT_TRUE(line.getParameter<std::string>(TEST_COMMAND_LINE_NAMEB) == TEST_COMMAND_LINE_PARAMB);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_mixedParameters)
{
	ghost::internal::CommandLineParser parser;
	ghost::CommandLine line = parser.parseCommandLine(TEST_COMMAND_LINE_STRING_WITH_MIXED);
	ASSERT_TRUE(line.getCommandName() == TEST_COMMAND_LINE_CMDNAME);
	ASSERT_TRUE(line.getParametersMap().size() == 3);
	ASSERT_TRUE(line.hasParameter(TEST_COMMAND_LINE_NAMEA));
	ASSERT_TRUE(line.hasParameter("__0"));
	ASSERT_TRUE(line.hasParameter("__1"));
	ASSERT_TRUE(line.getParameter<std::string>(TEST_COMMAND_LINE_NAMEA) == TEST_COMMAND_LINE_PARAMA);
	ASSERT_TRUE(line.getParameter<std::string>("__0") == TEST_COMMAND_LINE_PARAMC);
	ASSERT_TRUE(line.getParameter<std::string>("__1") == TEST_COMMAND_LINE_PARAMB);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_illegalParameters)
{
	ghost::internal::CommandLineParser parser;
	ghost::CommandLine line = parser.parseCommandLine(TEST_COMMAND_LINE_STRING_WITH_ILLEGAL);
	ASSERT_TRUE(line.getCommandName() == TEST_COMMAND_LINE_CMDNAME);
	ASSERT_TRUE(line.getParametersMap().size() == 2);

	std::string expectedNameCorrection = TEST_COMMAND_LINE_NAMEILLEGAL + "_1";

	ASSERT_TRUE(line.hasParameter("__0"));
	ASSERT_TRUE(line.hasParameter(expectedNameCorrection));
	ASSERT_TRUE(line.getParameter<std::string>("__0") == TEST_COMMAND_LINE_PARAMC);
	ASSERT_TRUE(line.getParameter<std::string>(expectedNameCorrection) == TEST_COMMAND_LINE_PARAMA);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_ok)
{
	auto interpreter = ghost::CommandLineInterpreter::create();
	ASSERT_TRUE(interpreter);

	auto command = std::make_shared<CustomCommand>(true);
	interpreter->registerCommand(command);
	bool executeSuccess = interpreter->execute(command->getShortcut());
	ASSERT_TRUE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_CommandLine)
{
	auto interpreter = ghost::CommandLineInterpreter::create();
	ASSERT_TRUE(interpreter);

	auto command = std::make_shared<CustomCommand>(true);
	interpreter->registerCommand(command);

	ghost::internal::CommandLineParser parser;
	ghost::CommandLine line = parser.parseCommandLine(command->getShortcut() + " -super parameter");

	bool executeSuccess = interpreter->execute(line);
	ASSERT_TRUE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_commandNotRegistered)
{
	auto interpreter = ghost::CommandLineInterpreter::create();
	ASSERT_TRUE(interpreter);

	bool executeSuccess = interpreter->execute(CustomCommand(true).getShortcut());
	ASSERT_FALSE(executeSuccess);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_commandFails)
{
	auto interpreter = ghost::CommandLineInterpreter::create();
	ASSERT_TRUE(interpreter);

	auto command = std::make_shared<CustomCommand>(false);
	interpreter->registerCommand(command);
	bool executeSuccess = interpreter->execute(command->getShortcut());
	ASSERT_FALSE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_help_containsInfo)
{
	auto interpreter = ghost::CommandLineInterpreter::create();
	ASSERT_TRUE(interpreter);

	auto command = std::make_shared<CustomCommand>(false);
	interpreter->registerCommand(command);

	std::ostringstream oss;
	interpreter->printHelp(oss);

	size_t nameFound = oss.str().find(command->getName());
	ASSERT_TRUE(nameFound != std::string::npos);

	size_t shortcutFound = oss.str().find(command->getShortcut());
	ASSERT_TRUE(shortcutFound != std::string::npos);

	size_t descriptionFound = oss.str().find(command->getDescription());
	ASSERT_TRUE(descriptionFound != std::string::npos);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_help_containsHelpAndLogin)
{
	auto interpreter = ghost::CommandLineInterpreter::create();
	ASSERT_TRUE(interpreter);

	std::ostringstream oss;
	interpreter->printHelp(oss);

	size_t nameFound = oss.str().find("HelpCommand");
	ASSERT_TRUE(nameFound != std::string::npos);

	size_t shortcutFound = oss.str().find("LoginCommand");
	ASSERT_TRUE(shortcutFound != std::string::npos);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_permissionsListIsEmpty)
{
	auto userManager = ghost::UserManager::create();
	auto interpreter = ghost::CommandLineInterpreter::create(userManager);
	ASSERT_TRUE(interpreter);

	auto command = std::make_shared<CustomCommand>(true);
	interpreter->registerCommand(command);
	bool executeSuccess = interpreter->execute(command->getShortcut());
	ASSERT_TRUE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_noUserManagerPermissionsListNotEmpty)
{
	auto interpreter = ghost::CommandLineInterpreter::create();
	ASSERT_TRUE(interpreter);

	std::list<std::shared_ptr<ghost::PermissionEntity>> permissionsList;
	auto user = std::make_shared<ghost::internal::User>("mathieu", "super");
	permissionsList.push_back(user);

	auto command = std::make_shared<CustomCommand>(true);
	interpreter->registerCommand(command, permissionsList);
	bool executeSuccess = interpreter->execute(command->getShortcut());
	ASSERT_TRUE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_userNotPermitted)
{
	auto userManager = ghost::UserManager::create();
	auto interpreter = ghost::CommandLineInterpreter::create(userManager);
	ASSERT_TRUE(interpreter);

	std::list<std::shared_ptr<ghost::PermissionEntity>> permissionsList;
	auto user = std::make_shared<ghost::internal::User>("mathieu", "super");
	permissionsList.push_back(user);

	userManager->createUser("mathieu2", "super2");
	userManager->connect("mathieu2", "super2");

	auto command = std::make_shared<CustomCommand>(true);
	interpreter->registerCommand(command, permissionsList);
	bool executeSuccess = interpreter->execute(command->getShortcut());
	ASSERT_FALSE(executeSuccess);
	ASSERT_FALSE(command->_executeWasCalled);
}
