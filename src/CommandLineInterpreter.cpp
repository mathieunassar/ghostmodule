#include "../include/internal/CommandLineInterpreter.hpp"
#include "../include/Command.hpp"
#include "../include/internal/CommandLineParser.hpp"

#include "../include/internal/commands/LoginCommand.hpp"
#include "../include/internal/commands/HelpCommand.hpp"

using namespace Ghost::internal;

std::shared_ptr<Ghost::CommandLineInterpreter> Ghost::CommandLineInterpreter::create(std::shared_ptr<UserManager> userManager)
{
	return std::shared_ptr<Ghost::CommandLineInterpreter>(new Ghost::internal::CommandLineInterpreter(userManager));
}

CommandLineInterpreter::CommandLineInterpreter(std::shared_ptr<Ghost::UserManager> userManager)
	: _userManager(userManager)
{
	registerCommand(std::shared_ptr<Ghost::Command>(new LoginCommand(userManager)), {});
	registerCommand(std::shared_ptr<Ghost::Command>(new HelpCommand(this)), {});
}

bool CommandLineInterpreter::execute(const std::string& commandLine)
{
	CommandLine cmd = CommandLineParser::parseCommandLine(commandLine);

	if (_commands.count(cmd.getCommandName()) > 0)
	{
		if (executionPermitted(_commands[cmd.getCommandName()]))
		{
			return _commands[cmd.getCommandName()].command->execute(cmd);
		}
		else
			throw std::logic_error("The user is not allowed to perform this operation");
	}

	return false;
}

void CommandLineInterpreter::registerCommand(std::shared_ptr<Ghost::Command> command, const std::list<std::shared_ptr<Ghost::PermissionEntity>>& permissions)
{
	CommandEntry entry;
	entry.command = command;
	entry.permissions = permissions;

	_commands[command->getShortcut()] = entry;
}

void CommandLineInterpreter::printHelp(std::ostream& stream) const
{
	std::ostringstream oss;
	oss << "List of registered commands:" << std::endl;
	for (auto it = _commands.begin(); it != _commands.end(); ++it)
	{
		if (executionPermitted(it->second))
		{
			oss << it->second.command->getName()
				<< " [" << it->second.command->getShortcut() << "] - "
				<< it->second.command->getDescription() << std::endl;
		}
	}
	stream << oss.str();
}

bool CommandLineInterpreter::executionPermitted(const CommandEntry& entry) const
{
	if (entry.permissions.empty())
		return true; // no permissions configured -> yes

	if (!_userManager->isUserConnected())
		return false; // some permissions were configured, but the user is not connected -> no

	Ghost::User& user = _userManager->getConnectedUser();

	if (user.getName() == "superuser")
		return true; // superuser can do everything

	for (auto& permit : entry.permissions)
	{
		if (permit->contains(user))
			return true; // either the user is explicitely allowed or contained by an allowed group -> yes
	}

	return false; // no permission found -> no
}
