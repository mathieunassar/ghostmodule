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

#ifndef GHOST_DATABASEFILE_HPP
#define GHOST_DATABASEFILE_HPP

#include <ghost/persistence/Database.hpp>
#include <map>
#include <memory>
#include <string>

namespace ghost
{
class DatabaseFile : public Database
{
public:
	virtual ~DatabaseFile() = default;

	static std::map<std::string, std::shared_ptr<ghost::DatabaseFile>> load(const std::string& root,
								    const std::list<std::string>& filenames);
	static std::shared_ptr<ghost::DatabaseFile> create(const std::string& filename);

	virtual bool save(bool overwrite) = 0;
};
} // namespace ghost

#endif // GHOST_DATABASEFILE_HPP
