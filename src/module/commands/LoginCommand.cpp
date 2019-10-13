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

#include "LoginCommand.hpp"

#include <iostream>
#include <ghost/module/GhostLogger.hpp>

using namespace ghost::internal;

const std::string LoginCommand::NAME = "LoginCommand";
const std::string LoginCommand::_SHORTCUT = "login";
const std::string LoginCommand::_DESCRIPTION = "changes the connected user.";
const std::string LoginCommand::_PARAM_USERNAME = "username";
const std::string LoginCommand::_PARAM_PASSWORD = "password";
const std::string LoginCommand::_PARAM_USERNAME_SHORT = "u";
const std::string LoginCommand::_PARAM_PASSWORD_SHORT = "p";

LoginCommand::LoginCommand(std::shared_ptr<ghost::UserManager> userManager) : _userManager(userManager)
{
}

bool LoginCommand::execute(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context)
{
	auto logger = ghost::GhostLogger::create(context.getConsole());
	std::string username, password;
	if (commandLine.hasParameter(_PARAM_USERNAME))
	{
		username = commandLine.getParameter<std::string>(_PARAM_USERNAME);
	}
	else if (commandLine.hasParameter(_PARAM_USERNAME_SHORT))
	{
		username = commandLine.getParameter<std::string>(_PARAM_USERNAME_SHORT);
	}

	if (commandLine.hasParameter(_PARAM_PASSWORD))
	{
		password = commandLine.getParameter<std::string>(_PARAM_PASSWORD);
	}
	else if (commandLine.hasParameter(_PARAM_PASSWORD_SHORT))
	{
		password = commandLine.getParameter<std::string>(_PARAM_PASSWORD_SHORT);
	}

	// here, if the module has a console and missing parameters, ask the user

	if (username.empty())
	{
		GHOST_INFO(logger) << "The username cannot be empty.";
		return false;
	}

	bool connectionSuccess = _userManager->connect(username, password, context.getSession());
	if (!connectionSuccess || !_userManager->isUserConnected(context.getSession()))
	{
		GHOST_INFO(logger) << "Connection failed: invalid username / password combination";
		return false;
	}

	auto user = _userManager->getConnectedUser(context.getSession());
	GHOST_INFO(logger) << "User " << user->getName() << " successfully connected";
	return true;
}

std::string LoginCommand::getName() const
{
	return NAME;
}

std::string LoginCommand::getShortcut() const
{
	return _SHORTCUT;
}

std::string LoginCommand::getDescription() const
{
	return _DESCRIPTION;
}
