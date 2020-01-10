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

#ifndef GHOST_MODULE_EXTENSION_BUILDER_HPP
#define GHOST_MODULE_EXTENSION_BUILDER_HPP

#include <ghost/module/ModuleComponent.hpp>
#include <memory>

namespace ghost
{
/**
 *	Builder for a specific module component.
 *	This builder can be added to the ghost::ModuleBuilder. Its "build" method
 *	will be called while building the module. This allows external components
 *	to be managed by the module, which owns them.
 */
class ModuleExtensionBuilder
{
public:
	virtual ~ModuleExtensionBuilder() = default;
	/**
	 *	Builds the module which this builder is the builder of.
	 *	@return a shared_ptr to the built module, downcasted to the public
	 *		interface ghost::ModuleExtension.
	 */
	virtual std::shared_ptr<ghost::ModuleExtension> build() = 0;
};

} // namespace ghost

#endif // GHOST_MODULE_EXTENSION_BUILDER_HPP
