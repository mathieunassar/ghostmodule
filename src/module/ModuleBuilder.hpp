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

#ifndef GHOST_INTERNAL_MODULE_BUILDER_HPP
#define GHOST_INTERNAL_MODULE_BUILDER_HPP

#include <ghost/module/ModuleBuilder.hpp>

#include "Console.hpp"
#include "Module.hpp"

namespace ghost
{
namespace internal
{
class ModuleBuilder : public ghost::ModuleBuilder
{
public:
	ModuleBuilder();

	// Module lifetime
	ModuleBuilder& setInitializeBehavior(const std::function<bool(const ghost::Module&)>& behavior) override;
	ModuleBuilder& setRunningBehavior(const std::function<bool(const ghost::Module&)>& behavior) override;
	ModuleBuilder& setDisposeBehavior(const std::function<void(const ghost::Module&)>& behavior) override;

	// Program options
	ModuleBuilder& setProgramOptions(int argc, char* argv[]) override;

	// Thread pools
	std::shared_ptr<ghost::ThreadPool> getThreadPool(const std::string& label = "") const override;
	std::shared_ptr<ghost::ThreadPool> addThreadPool(const std::string& label, size_t threadsCount) override;

	// Observability
	std::shared_ptr<ghost::Console> setConsole() override;
	ModuleBuilder& setLogger(const std::shared_ptr<ghost::Logger>& logger) override;

	// Extensions
	ModuleBuilder& addExtensionBuilder(const std::shared_ptr<ghost::ModuleExtensionBuilder>& builder) override;

	std::shared_ptr<ghost::Module> build(const std::string& moduleName = "") override;

private:
	std::function<bool(const ghost::Module&)> _initializationBehavior;
	std::function<bool(const ghost::Module&)> _runningBehavior;
	std::function<void(const ghost::Module&)> _disposeBehavior;
	std::vector<std::shared_ptr<ghost::ModuleExtensionBuilder>> _componentBuilders;
	ghost::CommandLine _options;
	std::map<std::string, std::shared_ptr<ThreadPool>> _threadPools;
	std::shared_ptr<Console> _console;
	std::shared_ptr<ghost::Logger> _logger;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_MODULE_HPP
