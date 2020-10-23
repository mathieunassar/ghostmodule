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

#include "ModuleBuilder.hpp"

#include "CommandLineParser.hpp"

using namespace ghost::internal;

std::unique_ptr<ghost::ModuleBuilder> ghost::ModuleBuilder::create()
{
	return std::make_unique<ghost::internal::ModuleBuilder>();
}

void ghost::ModuleBuilder::setModuleToExtension(const std::shared_ptr<ghost::Module>& module,
						const std::shared_ptr<ghost::ModuleExtension>& component)
{
	component->_module = module;
}

ModuleBuilder::ModuleBuilder() : _options("undefined")
{
	// This is the default thread pool
	_threadPools[""] = std::make_shared<ThreadPool>(std::thread::hardware_concurrency());
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

std::shared_ptr<ghost::ThreadPool> ModuleBuilder::getThreadPool(const std::string& label) const
{
	if (_threadPools.find(label) == _threadPools.end()) return nullptr;
	return _threadPools.at(label);
}

std::shared_ptr<ghost::ThreadPool> ModuleBuilder::addThreadPool(const std::string& label, size_t threadsCount)
{
	if (_threadPools.find(label) != _threadPools.end()) return _threadPools.at(label);

	_threadPools[label] = std::make_shared<ghost::internal::ThreadPool>(threadsCount);
	return _threadPools.at(label);
}

std::shared_ptr<ghost::Console> ModuleBuilder::setConsole()
{
	// the console gets the default thread pool
	_console = std::shared_ptr<Console>(new ghost::internal::Console(_threadPools[""]));
	return _console;
}

ModuleBuilder& ModuleBuilder::setLogger(const std::shared_ptr<ghost::Logger>& logger)
{
	_logger = logger;
	return *this;
}

ModuleBuilder& ModuleBuilder::addExtensionBuilder(const std::shared_ptr<ghost::ModuleExtensionBuilder>& builder)
{
	_componentBuilders.push_back(builder);
	return *this;
}

std::shared_ptr<ghost::Module> ModuleBuilder::build(const std::string& moduleName)
{
	std::vector<std::shared_ptr<ghost::ModuleExtension>> components;

	// Create the module components
	for (const auto& builder : _componentBuilders)
	{
		auto component = builder->build();
		if (!component) return nullptr;
		components.push_back(component);
	}

	// Create the module and give it the components
	auto module =
	    std::make_shared<ghost::internal::Module>(moduleName, _threadPools, _console, _logger, _options, components,
						      _initializationBehavior, _runningBehavior, _disposeBehavior);

	// Give a weak_ptr of the parent module to its components
	for (const auto& component : components)
	{
		// This can only be achieved because ghost::ModuleBuilder (base class of this)
		// is friend of ghost::ModuleExtension
		// This allows the public API to not expose a setter of the parent module
		// For that reason "setModuleToExtension is a protected static method of ghost::ModuleBuilder
		// and not of this internal class.
		setModuleToExtension(module, component);
	}

	return module;
}
