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
/**
 *	The data manager is the owner of all the databases registered in the application.
 *	It manages the lifetime of the databases by stopping them when the program ends (through its "stop()"
 *	method, which is called when the ghost::PersistenceExtension shuts down).
 *	It also generalizes the database concept by hiding the implementation details related to the database
 *	technology: managed databases can be files, connections to standalone databases (see also:
 *	https://github.com/mathieunassar/ghostmodule-mongodb) or anything else that fulfills the ghost::Database
 *	interface.
 *	Databases contain collections (\see{ghost::DataCollection}, which contain the data. Data is defined as
 *	Google Protobuf messages.
 */
class DataManager
{
public:
	/**
	 *	Creates a new data manager. If possible, register the ghost::PersistenceExtension to your module
	 *	instead and access the shared ghost::DataManager from the extension.
	 */
	static std::shared_ptr<ghost::DataManager> create();

	virtual ~DataManager() = default;
	/**
	 *	Stops the manager and closes all the databases that are currently managed.
	 */
	virtual void stop() = 0;
	/**
	 *	Registers a database to the data manager. The database can be retrieved by "getDatabase()" and it
	 *	is referenced by the provided "name".
	 *	@param database a database to manage
	 *	@param name name of the database to register
	 */
	virtual void addDatabase(std::shared_ptr<ghost::Database> database, const std::string& name) = 0;
	/**
	 *	@return the database registered under the provided name, or nullptr if no database was registered
	 *		with that name.
	 */
	virtual std::shared_ptr<ghost::Database> getDatabase(const std::string& name) const = 0;
	/**
	 *	Checks whether the data manager contains a database registered under the provided name.
	 *	@param name name of the database to check
	 *	@return true if a datbase was registered to the data manager with the provided name.
	 */
	virtual bool hasDatabase(const std::string& name) const = 0;
	/**
	 *	@return a list containing the names of all the databases registered to the data manager.
	 */
	virtual std::list<std::string> getDatabaseNames() const = 0;
	/**
	 *	Adds a collection to the database referenced by its name with the provided collection name.
	 *	This method finds the matching database and calls the method "addCollection" of the ghost::Database
	 *	interface.
	 *	@param database name of the database in which the collection must be added
	 *	@param name name of the collection to add
	 *	@return the newly created collection
	 */
	virtual std::shared_ptr<ghost::DataCollection> addCollection(const std::string& database,
								     const std::string& name) = 0;
	/**
	 *	Removed a named collection from the given database. This method finds the matching database and calls
	 *	the method "removeCollection" of the ghost::Database interface.
	 *	If the database parameter is left empty, collections matching the name will be removed from all the
	 *	managed databases.
	 *	@param name name of the collection to remove.
	 *	@param database name of a database in which the collection must be removed, or empty. If the parameter
	 *		is left empty, matching collections are removed from all the databases.
	 */
	virtual bool removeCollections(const std::string& name, const std::string& database = "") = 0;
	/**
	 *	Returns all the collections in all the databases that match the provided name. If the database parameter
	 *is not empty, returns all the collections in that database that match the provided name.
	 *	@param name name of the collections to find
	 *	@param database name of a database in which the collection must be found, or empty. If the parameter
	 *		is left empty, matching collections are found from all the databases.
	 */
	virtual std::map<std::string, std::list<std::shared_ptr<ghost::DataCollection>>> getCollections(
	    const std::string& name, const std::string& database = "") const = 0;
};
} // namespace ghost

#endif // GHOST_DATA_MANAGER_HPP
