#include <ghost/module/CommandLine.hpp>

using namespace ghost;

CommandLine::CommandLine(const std::string& commandName, std::map<std::string, std::string>& parameters)
	: _commandName(commandName)
	, _parameters(parameters)
{

}

const std::string& CommandLine::getCommandName() const
{
	return _commandName;
}

bool CommandLine::hasParameter(const std::string& parameterName) const
{
	return _parameters.count(parameterName) != 0;
}
