/*
 * Copyright 2020 Mathieu Nassar
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

#ifndef GHOST_TESTS_SYSTEMTESTCOMMAND_HPP
#define GHOST_TESTS_SYSTEMTESTCOMMAND_HPP

#include <ghost/module/Command.hpp>
#include <ghost/module/Console.hpp>

class SystemtestExecutorModule;

class SystemtestCommand : public ghost::Command
{
public:
	SystemtestCommand(SystemtestExecutorModule* parent, std::shared_ptr<ghost::Console> console)
	    : _parent(parent), _console(console)
	{
	}

	bool execute(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context) override;

	std::string getName() const override;
	std::string getShortcut() const override;
	std::string getDescription() const override;

private:
	SystemtestExecutorModule* _parent;
	std::shared_ptr<ghost::Console> _console;
};

#endif // GHOST_TESTS_SYSTEMTESTCOMMAND_HPP
