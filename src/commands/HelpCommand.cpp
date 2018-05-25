#include "../../include/internal/commands/HelpCommand.hpp"
#include <iostream>
#include "../../include/internal/CommandLineInterpreter.hpp"

using namespace ghost::internal;

const std::string HelpCommand::NAME = "HelpCommand";
const std::string HelpCommand::_SHORTCUT = "help";
const std::string HelpCommand::_DESCRIPTION = "Displays the list of available commands to the user";

HelpCommand::HelpCommand(CommandLineInterpreter* interpreter)
	: _interpreter(interpreter)
{

}

bool HelpCommand::execute(const ghost::CommandLine& commandLine)
{
	_interpreter->printHelp(std::cout);
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
