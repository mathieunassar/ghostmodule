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

ModuleBuilder::ModuleBuilder() : _options("undefined")
{
}

ModuleBuilder& ModuleBuilder::setInitializeBehavior(const std::function<bool(const ghost::Module&)>& behavior)
{
	_initializationBehavior = behavior;
	return *this;
}

ModuleBuilder& ModuleBuilder::setRunningBehavior(const std::function<bool(const ghost::Module&)>& behavior)
{
	_runningBehavior = behavior;
	return *this;
}

ModuleBuilder& ModuleBuilder::setDisposeBehavior(const std::function<void(const ghost::Module&)>& behavior)
{
	_disposeBehavior = behavior;
	return *this;
}

ModuleBuilder& ModuleBuilder::setProgramOptions(int argc, char* argv[])
{
	CommandLineParser parser;
	_options = parser.parseCommandLine(argc, argv);
	return *this;
}

std::shared_ptr<ghost::Console> ModuleBuilder::setConsole()
{
	_console = std::shared_ptr<Console>(new ghost::internal::Console());
	return _console;
}

ModuleBuilder& ModuleBuilder::setLogger(const std::shared_ptr<ghost::Logger>& logger)
{
	_logger = logger;
	return *this;
}

ModuleBuilder& ModuleBuilder::addComponentBuilder(const std::shared_ptr<ghost::ModuleComponentBuilder>& builder)
{
	_componentBuilders.push_back(builder);
	return *this;
}

std::shared_ptr<ghost::Module> ModuleBuilder::build(const std::string& moduleName)
{
	std::vector<std::shared_ptr<ghost::ModuleComponent>> components;

	for (const auto& builder : _componentBuilders)
	{
		auto component = builder->build();
		if (!component) return nullptr;
		components.push_back(component);
	}

	return std::make_shared<ghost::internal::Module>(moduleName, _console, _logger, _options, components,
							 _initializationBehavior, _runningBehavior, _disposeBehavior);
}
