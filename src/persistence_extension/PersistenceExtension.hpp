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

#ifndef GHOST_INTERNAL_PERSISTENCEEXTENSION_HPP
#define GHOST_INTERNAL_PERSISTENCEEXTENSION_HPP

#include <ghost/module/ModuleExtension.hpp>
#include <ghost/persistence_extension/PersistenceExtension.hpp>
#include <memory>

namespace ghost
{
namespace internal
{
class PersistenceExtension : public ghost::PersistenceExtension, public ghost::ModuleExtension
{
public:
	PersistenceExtension(const std::shared_ptr<ghost::DataManager>& dataManager);

	// From ghost::PersistenceExtension
	std::shared_ptr<ghost::DataManager> getDataManager() const override;

	// From ghost::ModuleExtension
	bool start() override;
	void stop() override;
	std::string getName() const override;

private:
	std::shared_ptr<ghost::DataManager> _dataManager;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_PERSISTENCEEXTENSION_HPP
