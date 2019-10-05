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

#include "ExitCommand.hpp"

#include <iostream>

#include "../Module.hpp"

using namespace ghost::internal;

const std::string ExitCommand::NAME = "ExitCommand";
const std::string ExitCommand::_SHORTCUT = "exit";
const std::string ExitCommand::_DESCRIPTION =
    "Switches the module state to 'DISPOSING'. The module will exit before the next call to run.";

ExitCommand::ExitCommand(ghost::internal::Module* module) : _module(module)
{
}

bool ExitCommand::execute(const ghost::CommandLine& commandLine)
{
	_module->setState(ghost::internal::Module::DISPOSING);
	_module->getConsole()->setInputMode(InputMode::DISCRETE); // do that to stop expecting user input
	return true;
}

std::string ExitCommand::getName() const
{
	return NAME;
}

std::string ExitCommand::getShortcut() const
{
	return _SHORTCUT;
}

std::string ExitCommand::getDescription() const
{
	return _DESCRIPTION;
}
