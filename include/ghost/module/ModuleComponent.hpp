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

#ifndef GHOST_MODULE_COMPONENT_HPP
#define GHOST_MODULE_COMPONENT_HPP

#include <memory>
#include <string>

namespace ghost
{
class Module;
class ModuleBuilder;
/**
 *	A module component is an extension of the module that is managed by the module
 *	workflow.
 *	For example, the Ghost library "ghost_connected_module" provides a component
 *	that adds connectivity to the module.
 *
 *	An implementation of a ghost::ModuleComponent must define a public member of type
 *	std::string called "NAME" to be usable by a ghost::Module object.
 */
class ModuleComponent
{
public:
	virtual ~ModuleComponent() = default;
	/**
	 *	Starts the component.
	 *	@return true if the component started successfully, false otherwise.
	 */
	virtual bool start() = 0;
	/**
	 *	Stops the component.
	 */
	virtual void stop() = 0;
	/**
	 *	@return the name of this component.
	 */
	virtual std::string getName() const = 0;

protected:
	/**
	 *	Retrieves the parent module. This method can be used to access core components
	 *	such as the ghost::CommandLineInterpreter.
	 *	@return this component's parent module.
	 */
	std::shared_ptr<ghost::Module> getModule() const;

private:
	std::weak_ptr<ghost::Module> _module;
	friend ghost::ModuleBuilder;
};

} // namespace ghost

#endif // GHOST_MODULE_COMPONENT_HPP
