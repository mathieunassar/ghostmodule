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

#include "PersistenceExtensionBuilder.hpp"

#include "PersistenceExtension.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::PersistenceExtensionBuilder> ghost::PersistenceExtensionBuilder::create()
{
	return std::make_shared<ghost::internal::PersistenceExtensionBuilder>();
}

PersistenceExtensionBuilder::PersistenceExtensionBuilder() : _dataManager(ghost::DataManager::create())
{
}

std::shared_ptr<ghost::DataManager> PersistenceExtensionBuilder::configureDataManager()
{
	return _dataManager;
}

std::shared_ptr<ghost::ModuleExtension> PersistenceExtensionBuilder::build()
{
	return std::make_shared<ghost::internal::PersistenceExtension>(_dataManager);
}
