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

#include "CommandLineParser.hpp"

#include <exception>

using namespace ghost::internal;

ghost::CommandLine CommandLineParser::parseCommandLine(const std::string& commandLine)
{
	if (commandLine.empty()) return ghost::CommandLine("", {});

	std::vector<std::string> nameAndParams;
	CommandLineParser::split(nameAndParams, commandLine, " ");

	return parseFromContainer(nameAndParams.size(), nameAndParams);
}

ghost::CommandLine CommandLineParser::parseCommandLine(int argc, char* argv[])
{
	return parseFromContainer(argc, argv);
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

bool CommandLineParser::isParameterName(const std::string& str, std::string& name)
{
	int startDashCount = 0;
	if (str.length() > 1 && str[0] == '-')
	{
		startDashCount++;
		if (str.length() > 2 && str[1] == '-') startDashCount++;
	}

	if (startDashCount > 0)
	{
		name = str.substr(startDashCount);
		return true;
	}

	return false;
}

void CommandLineParser::addParameter(std::map<std::string, std::string>& params, const std::string& name,
				     const std::string& value)
{
	std::string finalName = name;

	if (finalName.empty())
	{
		finalName = "__" + std::to_string(_unknownParametersCount);
		_unknownParametersCount++;
	}

	size_t count = params.count(finalName);
	size_t newcount = count;
	while (newcount != 0)
	{
		finalName = name + "_" + std::to_string(count);
		newcount = params.count(finalName);
		if (newcount > 0) count++;
	}

	params[finalName] = value;
}
