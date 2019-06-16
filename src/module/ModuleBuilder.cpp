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

#include "ModuleBuilder.hpp"
#include "CommandLineParser.hpp"

using namespace ghost::internal;

std::unique_ptr<ghost::ModuleBuilder> ghost::ModuleBuilder::create()
{
	return std::make_unique<ghost::internal::ModuleBuilder>();
}

ModuleBuilder::ModuleBuilder()
	: _options("undefined")
{

}

void ModuleBuilder::setInitializeBehavior(const std::function<bool(const ghost::Module&)>& behavior)
{
	_initializationBehavior = behavior;
}

void ModuleBuilder::setRunningBehavior(const std::function<bool(const ghost::Module&)>& behavior)
{
	_runningBehavior = behavior;
}

void ModuleBuilder::setDisposeBehavior(const std::function<void(const ghost::Module&)>& behavior)
{
	_disposeBehavior = behavior;
}

void ModuleBuilder::setProgramOptions(int argc, char* argv[])
{
	CommandLineParser parser;
	_options = parser.parseCommandLine(argc, argv);
}

std::shared_ptr<ghost::Console> ModuleBuilder::setConsole()
{
	_console = std::shared_ptr<Console>(new ghost::internal::Console());
	return _console;
}

void ModuleBuilder::setLogger(const std::shared_ptr<ghost::Logger>& logger)
{
	_logger = logger;
}

std::shared_ptr<ghost::Module> ModuleBuilder::build(const std::string& moduleName)
{
	return std::make_shared<ghost::internal::Module>(moduleName,
		_console, _logger, _options,
		_initializationBehavior, _runningBehavior, _disposeBehavior);
}
