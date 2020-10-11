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

#ifndef GHOST_INTERNAL_DATAMANAGER_HPP
#define GHOST_INTERNAL_DATAMANAGER_HPP

#include <ghost/persistence/DataManager.hpp>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace ghost
{
namespace internal
{
class DataManager : public ghost::DataManager
{
public:
	void addDatabase(std::shared_ptr<ghost::Database> database, const std::string& name) override;
	std::shared_ptr<ghost::Database> getDatabase(const std::string& name) const override;
	bool hasDatabase(const std::string& name) const override;
	std::list<std::string> getDatabaseNames() const override;

	std::shared_ptr<ghost::DataCollection> addCollection(const std::string& database,
							     const std::string& name) override;
	bool removeCollection(const std::string& name, const std::string& database = "") override;
	std::map<std::string, std::list<std::shared_ptr<ghost::DataCollection>>> getCollection(
	    const std::string& name, const std::string& database = "") const override;

private:
	std::map<std::string, std::shared_ptr<ghost::Database>> _databases;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_DATAMANAGER_HPP
