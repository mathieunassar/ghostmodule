#include "../../include/internal/commands/ExitCommand.hpp"
#include <iostream>
#include "../../include/internal/Module.hpp"

using namespace Ghost::internal;

const std::string ExitCommand::NAME = "ExitCommand";
const std::string ExitCommand::_SHORTCUT = "exit";
const std::string ExitCommand::_DESCRIPTION = "Switches the module state to 'DISPOSING'. The module will exit before the next call to run.";

ExitCommand::ExitCommand(Ghost::internal::Module* module)
	: _module(module)
{

}

bool ExitCommand::execute(const Ghost::CommandLine& commandLine)
{
	_module->setState(Ghost::internal::Module::DISPOSING);
	_module->getConsole()->setInputMode(Ghost::InputController::DISCRETE); // do that to stop expecting user input
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
