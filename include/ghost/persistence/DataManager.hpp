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

#ifndef GHOST_DATA_MANAGER_HPP
#define GHOST_DATA_MANAGER_HPP

#include <ghost/persistence/DataCollection.hpp>
#include <ghost/persistence/Database.hpp>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace ghost
{
class DataManager
{
public:
	static std::shared_ptr<ghost::DataManager> create();

	virtual ~DataManager() = default;

	virtual void addDatabase(std::shared_ptr<ghost::Database> database, const std::string& name) = 0;

	virtual std::shared_ptr<ghost::Database> getDatabase(const std::string& name) const = 0;

	virtual bool hasDatabase(const std::string& name) const = 0;

	virtual std::list<std::string> getDatabaseNames() const = 0;

	virtual std::shared_ptr<ghost::DataCollection> addCollection(const std::string& database,
								     const std::string& name) = 0;

	virtual bool removeCollection(const std::string& name, const std::string& database = "") = 0;

	virtual std::map<std::string, std::list<std::shared_ptr<ghost::DataCollection>>> getCollection(
	    const std::string& name, const std::string& database = "") const = 0;
};
} // namespace ghost

#endif // GHOST_DATA_MANAGER_HPP
