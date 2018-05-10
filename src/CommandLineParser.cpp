#include "../include/internal/CommandLineParser.hpp"

#include <exception>

using namespace Ghost::internal;

Ghost::CommandLine CommandLineParser::parseCommandLine(const std::string& commandLine)
{
	if (commandLine.empty())
		throw std::invalid_argument("the command line is empty");

	std::vector<std::string> nameAndParams;
	CommandLineParser::split(nameAndParams, commandLine, " ");
	
	std::map<std::string, std::string> params;
	std::string commandName = nameAndParams[0]; // since commandLine is not empty, this is guaranteed to exist

	std::string tmpParameterName;
	for (size_t i = 1; i < nameAndParams.size(); ++i)
	{
		std::string isParameterNameRes = isParameterName(nameAndParams[i]);
		if (!isParameterNameRes.empty()) // we are dealing with a new parameter
		{
			if (!tmpParameterName.empty()) // save the previous if there is one, this can only occur if previous had no value
			{
				addParameter(params, tmpParameterName, "");
			}

			// reset values
			tmpParameterName = isParameterNameRes;
		}
		else // we are dealing with a value, enter it and reset the values
		{
			if (tmpParameterName.empty()) // value has no parameter name
				tmpParameterName = "undefined";

			addParameter(params, tmpParameterName, nameAndParams[i]);

			// reset the values
			tmpParameterName = "";
		}
	}

	if (!tmpParameterName.empty()) // the last split element was a parameter
		addParameter(params, tmpParameterName, "");

	return Ghost::CommandLine(commandName, params);
}

void CommandLineParser::split(std::vector<std::string>& tokens, const std::string& str, const std::string& del)
{
	size_t d = 0, t = 0;
	tokens.clear();

	while (d < str.size() && t != std::string::npos)
	{
		t = str.find(del, d);
		tokens.push_back(str.substr(d, t - d));
		d = t + del.size();
	}
}

std::string CommandLineParser::createParameterName(const std::string prefix, int id)
{
	std::ostringstream oss;
	oss << prefix << id;
	return oss.str();
}

std::string CommandLineParser::isParameterName(const std::string& str)
{
	int startDashCount = 0;
	if (str.length() > 1 && str[0] == '-')
	{
		startDashCount++;
		if (str.length() > 2 && str[1] == '-')
			startDashCount++;
	}

	if (startDashCount > 0)
		return str.substr(startDashCount);
	return "";
}

void CommandLineParser::addParameter(std::map<std::string, std::string>& params, const std::string& name, const std::string& value)
{
	std::string finalName = name;
	
	size_t count = params.count(finalName);
	size_t newcount = count;
	while (newcount != 0)
	{
		finalName = createParameterName(name, count + 1);
		newcount = params.count(finalName);
		if (newcount > 0)
			count++;
	}

	params[finalName] = value;
}
