#include "../../include/internal/commands/LoginCommand.hpp"
#include <iostream>

using namespace ghost::internal;

const std::string LoginCommand::NAME = "LoginCommand";
const std::string LoginCommand::_SHORTCUT = "login";
const std::string LoginCommand::_DESCRIPTION = "changes the connected user.";
const std::string LoginCommand::_PARAM_USERNAME = "username";
const std::string LoginCommand::_PARAM_PASSWORD = "password";
const std::string LoginCommand::_PARAM_USERNAME_SHORT = "u";
const std::string LoginCommand::_PARAM_PASSWORD_SHORT = "p";

LoginCommand::LoginCommand(std::shared_ptr<ghost::UserManager> userManager)
	: _userManager(userManager)
{

}

bool LoginCommand::execute(const ghost::CommandLine& commandLine)
{
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
		std::cout << "The username cannot be empty." << std::endl;
		return false;
	}

	bool connectionSuccess = _userManager->connect(username, password);
	if (!connectionSuccess || !_userManager->isUserConnected())
	{
		std::cout << "Connection failed: invalid username / password combination" << std::endl;
		return false;
	}

	User& user = _userManager->getConnectedUser();
	std::cout << "User " << user.getName() << " successfully connected" << std::endl;
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
