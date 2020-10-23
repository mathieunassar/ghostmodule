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

#ifndef GHOST_DATABASE_HPP
#define GHOST_DATABASE_HPP

#include <ghost/persistence/DataCollection.hpp>
#include <list>
#include <memory>
#include <string>

namespace ghost
{
/**
 *	A ghost::Database is a generic interface for a container for collections of data.
 *	Regardless of the database technology, this interface unifies the interactions with databases.
 *	Databases contain collections, which in turn contain the data.
 *	Databases can be opened or closed.
 *	In order to use this interface, users must create a ghost::Database from a class of the desired
 *	technology. See \see{ghost::DatabaseFile} for an example of such a class.
 */
class Database
{
public:
	virtual ~Database() = default;
	/**
	 *	Opens the database in order to start reading/writing from it.
	 *	Depending on the implementation, this could mean reading from a file (\see{ghost::DatabaseFile},
	 *	or opening a connection to a standalone database.
	 *	@return true if the open operation succeeds, false otherwise.
	 */
	virtual bool open() = 0;
	/**
	 *	Closes the database. Simlarly to "open()", this operation may shut down a connection or close a file.
	 */
	virtual void close() = 0;
	/**
	 *	Adds a collection of data to the database, which can afterwards be refered to by its provided name.
	 *	The name does not have to be unique, although specific implementations may have this limitation.
	 *	Calls to "getCollection()" return the list of all collections of the same name.
	 *	@param name	name of the data collection to add to the database.
	 *	@return the newly added collection.
	 */
	virtual std::shared_ptr<ghost::DataCollection> addCollection(const std::string& name) = 0;
	/**
	 *	Removes all the collections in the database with the provided name.
	 *	@param name	name of the collections to remove
	 *	@return true if at least one collection has been removed, false otherwise.
	 */
	virtual bool removeCollections(const std::string& name) = 0;
	/**
	 *	Finds and returns all the collections in the database with the provided name.
	 *	@param name name of the data collection to retrieve
	 *	@return the list of all collections whose name matched the provided value.
	 */
	virtual std::list<std::shared_ptr<ghost::DataCollection>> getCollections(const std::string& name) const = 0;
};
} // namespace ghost

#endif // GHOST_DATABASE_HPP
