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

#ifndef GHOST_PERSISTENCEEXTENSIONBUILDER_HPP
#define GHOST_PERSISTENCEEXTENSIONBUILDER_HPP

#include <ghost/module/ModuleExtensionBuilder.hpp>
#include <ghost/persistence/DataManager.hpp>
#include <memory>

namespace ghost
{
/**
 *	Builder for ghost::PersistenceExtension.
 *	To use this extension, create an instance of this builder with the help of "create()"
 *	and pass it to the ghost::ModuleBuilder instance at the configuration phase of the program.
 */
class PersistenceExtensionBuilder : public ghost::ModuleExtensionBuilder
{
public:
	static std::shared_ptr<ghost::PersistenceExtensionBuilder> create();

	virtual ~PersistenceExtensionBuilder() = default;
	/**
	 *	Returns the instance of the ghost::DataManager used by the component. When using this builder, the
	 *	ghost::ConnectionManager must be configured before calling "build" on the owning ghost::ModuleBuilder.
	 *	@return the instance of ghost::DataManager used by the component. This instance
	 *		may be used to register instances of ghost::Database. The data manager
	 *		exposes registered database to the rest of the module and manages their lifetime.
	 */
	virtual std::shared_ptr<ghost::DataManager> configureDataManager() = 0;
};
} // namespace ghost

#endif // GHOST_PERSISTENCEEXTENSIONBUILDER_HPP
