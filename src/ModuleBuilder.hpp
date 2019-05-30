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

#ifndef GHOST_INTERNAL_MODULE_BUILDER_HPP
#define GHOST_INTERNAL_MODULE_BUILDER_HPP

#include <ghost/module/ModuleBuilder.hpp>
#include "Module.hpp"
#include "Console.hpp"

namespace ghost
{
	namespace internal
	{
		class ModuleBuilder : public ghost::ModuleBuilder
		{
		public:
			ModuleBuilder();

			void setInitializeBehavior(const std::function<bool(const ghost::Module&)>& behavior) override;
			void setRunningBehavior(const std::function<bool(const ghost::Module&)>& behavior) override;
			void setDisposeBehavior(const std::function<void(const ghost::Module&)>& behavior) override;
			void setProgramOptions(int argc, char* argv[]) override;
			std::shared_ptr<ghost::Console> setConsole() override;
			void setLogger(const std::shared_ptr<ghost::Logger>& logger) override;

			std::shared_ptr<ghost::Module> build(const std::string& moduleName = "") override;

		private:
			std::function<bool(const ghost::Module&)> _initializationBehavior;
			std::function<bool(const ghost::Module&)> _runningBehavior;
			std::function<void(const ghost::Module&)> _disposeBehavior;
			ghost::CommandLine _options;
			std::shared_ptr<Console> _console;
			std::shared_ptr<ghost::Logger> _logger;
		};
	}
}

#endif // GHOST_MODULE_HPP
