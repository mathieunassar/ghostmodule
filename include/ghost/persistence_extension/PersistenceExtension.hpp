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

#ifndef GHOST_PERSISTENCEEXTENSION_HPP
#define GHOST_PERSISTENCEEXTENSION_HPP

#include <ghost/persistence/DataManager.hpp>
#include <memory>
#include <string>

namespace ghost
{
/**
 *	The persistence extension can be used to let the ghost::Module manage the lifetime
 *	of databases and to offer an access to the data system to the rest of the system.
 *	This extension owns an instance of the ghost::DataManager, which is used to register the
 *	databases to use in the application.
 */
class PersistenceExtension
{
public:
	/// Name of this component. Used by ghost::Module to identify this component.
	static const std::string getExtensionName();

	virtual ~PersistenceExtension() = default;

	/**
	 *	@return the instance of ghost::DataManager used by the component. This instance
	 *		may be used to register instances of ghost::Database. The data manager
	 *		exposes registered database to the rest of the module and manages their lifetime.
	 */
	virtual std::shared_ptr<ghost::DataManager> getDataManager() const = 0;
};
} // namespace ghost

#endif // GHOST_PERSISTENCEEXTENSION_HPP
