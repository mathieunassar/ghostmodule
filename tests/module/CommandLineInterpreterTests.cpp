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

#include <gtest/gtest.h>

#include <ghost/module/Command.hpp>
#include <ghost/module/CommandLineInterpreter.hpp>
#include <ghost/module/StdoutLogger.hpp>
#include <ghost/module/UserManager.hpp>

#include "../src/module/CommandLineParser.hpp"
#include "../src/module/User.hpp"

class CommandLineInterpreterTest : public testing::Test
{
protected:
	void SetUp() override
	{
		_userManager = ghost::UserManager::create();
		_interpreter = ghost::CommandLineInterpreter::create(_userManager);

		ASSERT_TRUE(_userManager);
		ASSERT_TRUE(_interpreter);
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

	ghost::internal::CommandLineParser _parser;
	std::shared_ptr<ghost::UserManager> _userManager;
	std::shared_ptr<ghost::CommandLineInterpreter> _interpreter;
};

class CustomCommand : public ghost::Command
{
public:
	CustomCommand(bool returnResult)
	    : _returnResult(returnResult), _executeWasCalled(false), _hadConsole(false), _lastSessionId("")
	{
	}

	bool execute(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context) override
	{
		if (context.getConsole()) _hadConsole = true;
		if (context.getSession()) _lastSessionId = context.getSession()->getUUID();
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
	bool _hadConsole;
	std::string _lastSessionId;
};

const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_CMDNAME = "testCommand";

const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_PARAMA = "paramA";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_PARAMB = "paramB";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_PARAMC = "paramC";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_STRING =
    TEST_COMMAND_LINE_CMDNAME + " " + TEST_COMMAND_LINE_PARAMA + " " + TEST_COMMAND_LINE_PARAMB + " " +
    TEST_COMMAND_LINE_PARAMC;

const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_NAMEA = "nameA";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_NAMEB = "nameB";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_STRING_WITH_PARAMNAME =
    TEST_COMMAND_LINE_CMDNAME + " --" + TEST_COMMAND_LINE_NAMEA + " " + TEST_COMMAND_LINE_PARAMA + " --" +
    TEST_COMMAND_LINE_NAMEB + " " + TEST_COMMAND_LINE_PARAMB;
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_STRING_WITH_MIXED =
    TEST_COMMAND_LINE_CMDNAME + " " + TEST_COMMAND_LINE_PARAMC + " --" + TEST_COMMAND_LINE_NAMEA + " " +
    TEST_COMMAND_LINE_PARAMA + " " + TEST_COMMAND_LINE_PARAMB;

const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_NAMEILLEGAL = "__0";
const std::string CommandLineInterpreterTest::TEST_COMMAND_LINE_STRING_WITH_ILLEGAL =
    TEST_COMMAND_LINE_CMDNAME + " " + TEST_COMMAND_LINE_PARAMC + " --" + TEST_COMMAND_LINE_NAMEILLEGAL + " " +
    TEST_COMMAND_LINE_PARAMA;

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_emptyLine)
{
	auto commandLine = _parser.parseCommandLine("");
	ASSERT_TRUE(commandLine.getCommandName().empty());
	ASSERT_TRUE(commandLine.getParametersMap().empty());
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_unknownParameters)
{
	ghost::CommandLine line = _parser.parseCommandLine(TEST_COMMAND_LINE_STRING);
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
	ghost::CommandLine line = _parser.parseCommandLine(TEST_COMMAND_LINE_CMDNAME);
	ASSERT_TRUE(line.getCommandName() == TEST_COMMAND_LINE_CMDNAME);
	ASSERT_TRUE(line.getParametersMap().size() == 0);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_namedParameters)
{
	ghost::CommandLine line = _parser.parseCommandLine(TEST_COMMAND_LINE_STRING_WITH_PARAMNAME);
	ASSERT_TRUE(line.getCommandName() == TEST_COMMAND_LINE_CMDNAME);
	ASSERT_TRUE(line.getParametersMap().size() == 2);
	ASSERT_TRUE(line.hasParameter(TEST_COMMAND_LINE_NAMEA));
	ASSERT_TRUE(line.hasParameter(TEST_COMMAND_LINE_NAMEB));
	ASSERT_TRUE(line.getParameter<std::string>(TEST_COMMAND_LINE_NAMEA) == TEST_COMMAND_LINE_PARAMA);
	ASSERT_TRUE(line.getParameter<std::string>(TEST_COMMAND_LINE_NAMEB) == TEST_COMMAND_LINE_PARAMB);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineParser_When_mixedParameters)
{
	ghost::CommandLine line = _parser.parseCommandLine(TEST_COMMAND_LINE_STRING_WITH_MIXED);
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
	ghost::CommandLine line = _parser.parseCommandLine(TEST_COMMAND_LINE_STRING_WITH_ILLEGAL);
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
	auto command = std::make_shared<CustomCommand>(true);
	_interpreter->registerCommand(command);
	bool executeSuccess = _interpreter->execute(command->getShortcut(),
						    ghost::CommandExecutionContext(ghost::Session::createLocal()));
	ASSERT_TRUE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_CommandLine)
{
	auto command = std::make_shared<CustomCommand>(true);
	_interpreter->registerCommand(command);

	ghost::CommandLine line = _parser.parseCommandLine(command->getShortcut() + " -super parameter");

	bool executeSuccess =
	    _interpreter->execute(line, ghost::CommandExecutionContext(ghost::Session::createLocal()));
	ASSERT_TRUE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_commandNotRegistered)
{
	bool executeSuccess = _interpreter->execute(CustomCommand(true).getShortcut(),
						    ghost::CommandExecutionContext(ghost::Session::createLocal()));
	ASSERT_FALSE(executeSuccess);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_commandFails)
{
	auto command = std::make_shared<CustomCommand>(false);
	_interpreter->registerCommand(command);
	bool executeSuccess = _interpreter->execute(command->getShortcut(),
						    ghost::CommandExecutionContext(ghost::Session::createLocal()));
	ASSERT_FALSE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_help_containsInfo)
{
	auto command = std::make_shared<CustomCommand>(false);
	_interpreter->registerCommand(command);

	std::ostringstream oss;
	_interpreter->printHelp(oss, ghost::Session::createLocal());

	size_t nameFound = oss.str().find(command->getName());
	ASSERT_TRUE(nameFound != std::string::npos);

	size_t shortcutFound = oss.str().find(command->getShortcut());
	ASSERT_TRUE(shortcutFound != std::string::npos);

	size_t descriptionFound = oss.str().find(command->getDescription());
	ASSERT_TRUE(descriptionFound != std::string::npos);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_help_containsHelpAndLogin)
{
	std::ostringstream oss;
	_interpreter->printHelp(oss, ghost::Session::createLocal());

	size_t nameFound = oss.str().find("HelpCommand");
	ASSERT_TRUE(nameFound != std::string::npos);

	size_t shortcutFound = oss.str().find("LoginCommand");
	ASSERT_TRUE(shortcutFound != std::string::npos);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_permissionsListIsEmpty)
{
	auto command = std::make_shared<CustomCommand>(true);
	_interpreter->registerCommand(command);
	bool executeSuccess = _interpreter->execute(command->getShortcut(),
						    ghost::CommandExecutionContext(ghost::Session::createLocal()));
	ASSERT_TRUE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_noUserManagerPermissionsListNotEmpty)
{
	// recreate the interpreter without user manager for this test.
	_interpreter = ghost::CommandLineInterpreter::create();

	std::list<std::shared_ptr<ghost::PermissionEntity>> permissionsList;
	auto user = std::make_shared<ghost::internal::User>("mathieu", "super");
	permissionsList.push_back(user);

	auto command = std::make_shared<CustomCommand>(true);
	_interpreter->registerCommand(command, permissionsList);
	bool executeSuccess = _interpreter->execute(command->getShortcut(),
						    ghost::CommandExecutionContext(ghost::Session::createLocal()));
	ASSERT_TRUE(executeSuccess);
	ASSERT_TRUE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_execute_When_userNotPermitted)
{
	std::list<std::shared_ptr<ghost::PermissionEntity>> permissionsList;
	auto user = std::make_shared<ghost::internal::User>("mathieu", "super");
	permissionsList.push_back(user);

	_userManager->createUser("mathieu2", "super2");
	_userManager->connect("mathieu2", "super2", ghost::Session::createLocal());

	auto command = std::make_shared<CustomCommand>(true);
	_interpreter->registerCommand(command, permissionsList);
	bool executeSuccess = _interpreter->execute(command->getShortcut(),
						    ghost::CommandExecutionContext(ghost::Session::createLocal()));
	ASSERT_FALSE(executeSuccess);
	ASSERT_FALSE(command->_executeWasCalled);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_executeCanAccessContextConsole)
{
	auto command = std::make_shared<CustomCommand>(true);
	_interpreter->registerCommand(command);
	ghost::CommandExecutionContext context(ghost::Session::createLocal());
	context.setConsole(ghost::Console::create());

	_interpreter->execute(command->getShortcut(), context);
	ASSERT_TRUE(command->_hadConsole);
}

TEST_F(CommandLineInterpreterTest, Test_CommandLineInterpreter_executeCanAccessContextSession)
{
	auto command = std::make_shared<CustomCommand>(true);
	_interpreter->registerCommand(command);
	auto session = ghost::Session::create();
	ghost::CommandExecutionContext context(session);

	_interpreter->execute(command->getShortcut(), ghost::CommandExecutionContext(session));
	ASSERT_TRUE(command->_lastSessionId == session->getUUID());
}
