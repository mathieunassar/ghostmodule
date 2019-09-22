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

#include "SystemtestCommand.hpp"
#include "SystemtestExecutorModule.hpp"

bool SystemtestCommand::execute(const ghost::CommandLine& commandLine)
{
	if (commandLine.hasParameter("__0"))
	{
		std::string testName = commandLine.getParameter<std::string>("__0");
		Systemtest::Parameters parameters;
		parameters.commandLine = commandLine;

		if (commandLine.hasParameter("duration"))
		{
			long long duration = commandLine.getParameter<long long>("duration");
			parameters.duration = std::chrono::seconds(duration);
		}
		else
			parameters.duration = std::chrono::seconds(0);

		_parent->executeSystemtest(testName, parameters);
		return true;
	}
	return false;
}

std::string SystemtestCommand::getName() const
{
	return "SystemtestCommand";
}

std::string SystemtestCommand::getShortcut() const
{
	return "systest";
}

std::string SystemtestCommand::getDescription() const
{
	return "Executes a systemtest with the given name and parameters.";
}
