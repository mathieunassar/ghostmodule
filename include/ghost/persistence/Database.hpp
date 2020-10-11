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

#ifndef GHOST_DATABASE_HPP
#define GHOST_DATABASE_HPP

#include <ghost/persistence/DataCollection.hpp>
#include <list>
#include <memory>
#include <string>

namespace ghost
{
class Database
{
public:
	virtual ~Database() = default;

	virtual bool open() = 0;

	virtual void close() = 0;

	virtual std::shared_ptr<ghost::DataCollection> addCollection(const std::string& name) = 0;

	virtual bool removeCollection(const std::string& name) = 0;

	virtual std::list<std::shared_ptr<ghost::DataCollection>> getCollection(const std::string& name) const = 0;
};
} // namespace ghost

#endif // GHOST_DATABASE_HPP
