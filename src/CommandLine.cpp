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
