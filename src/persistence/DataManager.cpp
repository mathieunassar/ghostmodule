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

#include "DataManager.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::DataManager> ghost::DataManager::create()
{
	return std::make_shared<ghost::internal::DataManager>();
}

void DataManager::addDatabase(std::shared_ptr<ghost::Database> database, const std::string& name)
{
	_databases[name] = database;
}

std::shared_ptr<ghost::Database> DataManager::getDatabase(const std::string& name) const
{
	if (_databases.find(name) == _databases.end()) return nullptr;
	return _databases.at(name);
}

bool DataManager::hasDatabase(const std::string& name) const
{
	return _databases.find(name) != _databases.end();
}

std::list<std::string> DataManager::getDatabaseNames() const
{
	std::list<std::string> names;
	for (const auto& database : _databases) names.push_back(database.first);
	return names;
}

std::shared_ptr<ghost::DataCollection> DataManager::addCollection(const std::string& database, const std::string& name)
{
	if (_databases.find(database) == _databases.end()) return nullptr;

	return _databases.at(database)->addCollection(name);
}

bool DataManager::removeCollection(const std::string& name, const std::string& database)
{
	if (!database.empty())
	{
		if (_databases.find(database) == _databases.end()) return false;

		return _databases.at(database)->removeCollection(name);
	}

	// else
	bool removedSome = false;
	for (const auto& database : _databases) removedSome = removedSome || database.second->removeCollection(name);
	return removedSome;
}

std::map<std::string, std::list<std::shared_ptr<ghost::DataCollection>>> DataManager::getCollection(
    const std::string& name, const std::string& database) const
{
	if (!database.empty())
	{
		if (_databases.find(database) == _databases.end()) return {};

		auto result = _databases.at(database)->getCollection(name);
		return {{database, result}};
	}

	std::map<std::string, std::list<std::shared_ptr<ghost::DataCollection>>> result;
	for (const auto& database : _databases)
	{
		auto found = database.second->getCollection(name);
		if (found.size() != 0) result[database.first] = found;
	}

	return result;
}
