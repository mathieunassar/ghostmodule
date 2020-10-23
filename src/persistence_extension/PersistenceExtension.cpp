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

#include "PersistenceExtension.hpp"

#include <ghost/module/Module.hpp>

using namespace ghost::internal;

const std::string ghost::PersistenceExtension::getExtensionName()
{
	return "PersistenceExtension";
}

PersistenceExtension::PersistenceExtension(const std::shared_ptr<ghost::DataManager>& dataManager)
    : _dataManager(dataManager)
{
}

std::shared_ptr<ghost::DataManager> PersistenceExtension::getDataManager() const
{
	return _dataManager;
}

bool PersistenceExtension::start()
{
	return true;
}

void PersistenceExtension::stop()
{
	_dataManager->stop();
}

std::string PersistenceExtension::getName() const
{
	return ghost::PersistenceExtension::getExtensionName();
}
