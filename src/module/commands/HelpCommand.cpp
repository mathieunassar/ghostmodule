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

#include "HelpCommand.hpp"

#include <iostream>

#include <ghost/module/GhostLogger.hpp>
#include "../CommandLineInterpreter.hpp"

using namespace ghost::internal;

const std::string HelpCommand::NAME = "HelpCommand";
const std::string HelpCommand::_SHORTCUT = "help";
const std::string HelpCommand::_DESCRIPTION = "Displays the list of available commands to the user";

HelpCommand::HelpCommand(CommandLineInterpreter* interpreter) : _interpreter(interpreter)
{
}

bool HelpCommand::execute(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context)
{
	auto logger = ghost::GhostLogger::create(context.getConsole());

	std::ostringstream oss;
	_interpreter->printHelp(oss, context.getSession());

	GHOST_INFO(logger) << oss.str();

	return true;
}

std::string HelpCommand::getName() const
{
	return NAME;
}

std::string HelpCommand::getShortcut() const
{
	return _SHORTCUT;
}

std::string HelpCommand::getDescription() const
{
	return _DESCRIPTION;
}
