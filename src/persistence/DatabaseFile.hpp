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

#ifndef GHOST_INTERNAL_DATABASEFILE_HPP
#define GHOST_INTERNAL_DATABASEFILE_HPP

#include <ghost/persistence/DatabaseFile.hpp>
#include "DataCollectionFile.hpp"

namespace ghost
{
namespace internal
{
class DatabaseFile : public ghost::DatabaseFile
{
public:
	DatabaseFile(const std::string& filename);

	bool open() override;
	void close() override;

	std::shared_ptr<ghost::DataCollection> addCollection(const std::string& name) override;
	bool removeCollections(const std::string& name) override;
	std::list<std::shared_ptr<ghost::DataCollection>> getCollections(const std::string& name) const override;

	bool save(bool overwrite) override;

private:
	std::string _filename;
	std::list<std::shared_ptr<ghost::internal::DataCollectionFile>> _data;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_DATABASEFILE_HPP
