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

#include "CommandLineInterpreter.hpp"

#include <ghost/module/Command.hpp>

#include <algorithm>
#include <iomanip>
#include "CommandLineParser.hpp"
#include "commands/HelpCommand.hpp"
#include "commands/LoginCommand.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::CommandLineInterpreter> ghost::CommandLineInterpreter::create(
    std::shared_ptr<ghost::Logger> logger)
{
	return std::shared_ptr<ghost::CommandLineInterpreter>(
	    new ghost::internal::CommandLineInterpreter(nullptr, logger));
}

std::shared_ptr<ghost::CommandLineInterpreter> ghost::CommandLineInterpreter::create(
    std::shared_ptr<UserManager> userManager, std::shared_ptr<ghost::Logger> logger)
{
	return std::shared_ptr<ghost::CommandLineInterpreter>(
	    new ghost::internal::CommandLineInterpreter(userManager, logger));
}

CommandLineInterpreter::CommandLineInterpreter(std::shared_ptr<ghost::UserManager> userManager,
					       std::shared_ptr<ghost::Logger> logger)
    : _userManager(userManager), _logger(logger)
{
	registerCommand(std::shared_ptr<ghost::Command>(new HelpCommand(this)), {});
	registerCommand(std::shared_ptr<ghost::Command>(new LoginCommand(userManager)), {});
}

bool CommandLineInterpreter::execute(const std::string& commandLine, const ghost::CommandExecutionContext& context)
{
	CommandLineParser parser;
	CommandLine cmd = parser.parseCommandLine(commandLine);

	return execute(cmd, context);
}

bool CommandLineInterpreter::execute(const ghost::CommandLine& commandLine,
				     const ghost::CommandExecutionContext& context)
{
	if (_commands.count(commandLine.getCommandName()) > 0)
	{
		const auto& entry = _commands[commandLine.getCommandName()];
		bool permitted = executionPermitted(entry, context.getSession());
		if (!permitted)
		{
			GHOST_ERROR(_logger) << commandLine.getCommandName() << ": execution not permitted.";
			return false;
		}

		bool validated = validate(entry, commandLine);
		if (!validated)
		{
			GHOST_ERROR(_logger) << commandLine.getCommandName() << ": usage: " << makeUsageString(entry);
			return false;
		}

		return entry.command->execute(commandLine, context);
	}

	GHOST_ERROR(_logger) << commandLine.getCommandName()
			     << ": command not found (enter \"help\" for a full list of available commands).";
	return false;
}

void CommandLineInterpreter::registerCommand(std::shared_ptr<ghost::Command> command,
					     const std::list<std::shared_ptr<ghost::PermissionEntity>>& permissions)
{
	CommandEntry entry;
	entry.command = command;
	entry.permissions = permissions;

	_commands[command->getShortcut()] = entry;
}

void CommandLineInterpreter::printHelp(std::ostream& stream, const std::shared_ptr<ghost::Session>& session) const
{
	std::ostringstream oss;
	oss << "List of registered commands:" << std::endl;

	// Sort the commands by categories
	std::vector<std::pair<std::string, CommandEntry>> asVector(_commands.begin(), _commands.end());
	std::sort(asVector.begin(), asVector.end(),
		  [](const std::pair<std::string, CommandEntry>& a, const std::pair<std::string, CommandEntry>& b) {
			  return a.second.command->getCategory() < b.second.command->getCategory();
		  });

	std::string lastCategory = "";
	for (auto command : asVector)
	{
		if (executionPermitted(command.second, session))
		{
			if (command.second.command->getCategory() != lastCategory)
			{
				lastCategory = command.second.command->getCategory();
				oss << "------------ Category: " << lastCategory << " ------------" << std::endl;
			}
			oss << std::left << std::setw(30)
			    << (command.second.command->getName() + " [" + command.second.command->getShortcut() + "]")
			    << std::setw(50) << command.second.command->getDescription() << std::endl;
		}
	}

	stream << oss.str();
}

bool CommandLineInterpreter::printCommandHelp(std::ostream& stream, const std::string& commandName,
					      const std::shared_ptr<ghost::Session>& session) const
{
	// Check that the command exists
	if (_commands.find(commandName) == _commands.end()) return false;
	// Check that the user is allowed to see it
	if (!executionPermitted(_commands.at(commandName), session)) return false;

	std::ostringstream oss;
	const auto& cmd = _commands.at(commandName).command;
	oss << "Name: " << cmd->getName() << " [" << cmd->getShortcut() << "]" << std::endl;
	oss << "Description: " << cmd->getDescription() << std::endl;

	oss << "Required parameters:" << std::endl;
	for (auto parameter : cmd->getRequiredParameters())
	{
		oss << makeParameterUsageString(parameter) << std::endl;
		oss << "\t" << parameter.getDescription() << std::endl;
	}

	oss << "Optional parameters:" << std::endl;
	for (auto parameter : cmd->getOptionalParameters())
	{
		oss << makeParameterUsageString(parameter) << std::endl;
		oss << "\t" << parameter.getDescription() << std::endl;
	}

	stream << oss.str();
	return true;
}

bool CommandLineInterpreter::executionPermitted(const CommandEntry& entry,
						const std::shared_ptr<ghost::Session>& session) const
{
	if (!_userManager) return true;

	if (entry.permissions.empty()) return true; // no permissions configured -> yes

	if (!_userManager->isUserConnected(session))
		return false; // some permissions were configured, but the user is not connected -> no

	auto user = _userManager->getConnectedUser(session);

	for (auto& permit : entry.permissions)
	{
		if (permit->contains(*user))
			return true; // either the user is explicitely allowed or contained by an allowed group -> yes
	}

	return false; // no permission found -> no
}

bool CommandLineInterpreter::validate(const CommandEntry& entry, const ghost::CommandLine& commandLine) const
{
	auto requiredParameters = entry.command->getRequiredParameters();
	bool isValid = true;

	size_t parameterPosition = 0;
	for (auto parameter : requiredParameters)
	{
		if (parameter.getIdentifier().empty() && parameter.getShortIdentifier().empty())
			isValid = isValid && commandLine.hasParameter("__" + std::to_string(parameterPosition));
		else
			isValid = isValid && (commandLine.hasParameter(parameter.getIdentifier()) ||
					      commandLine.hasParameter(parameter.getShortIdentifier()));
		++parameterPosition;
	}

	return isValid;
}

std::string CommandLineInterpreter::makeUsageString(const CommandEntry& entry) const
{
	auto requiredParameters = entry.command->getRequiredParameters();
	auto optionalParameters = entry.command->getOptionalParameters();

	std::ostringstream oss;
	oss << entry.command->getShortcut();

	for (auto parameter : requiredParameters) oss << " " << makeParameterUsageString(parameter);
	for (auto parameter : optionalParameters) oss << " [" << makeParameterUsageString(parameter) << "]";

	return oss.str();
}

std::string CommandLineInterpreter::makeParameterUsageString(const ghost::CommandParameter& parameter) const
{
	std::ostringstream oss;
	if (parameter.getIdentifier().empty() && parameter.getShortIdentifier().empty())
		oss << "<" << parameter.getName() << ">";
	else if (parameter.getShortIdentifier().empty())
	{
		oss << "-" << parameter.getIdentifier();
		if (parameter.expectsValue()) oss << " <" << parameter.getName() << ">";
	}
	else if (parameter.getIdentifier().empty())
	{
		oss << "--" << parameter.getShortIdentifier();
		if (parameter.expectsValue()) oss << " <" << parameter.getName() << ">";
	}
	else // both are provided
	{
		oss << "-" << parameter.getIdentifier() << "|--" << parameter.getShortIdentifier();
		if (parameter.expectsValue()) oss << " <" << parameter.getName() << ">";
	}
	return oss.str();
}
