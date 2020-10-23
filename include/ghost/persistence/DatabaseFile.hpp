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
/**
 *	Implementation of a ghost::Database based on files.
 *	File databases are stored on the disk at the configured location. The data is serialized
 *	with Google Protobuf's serialization format. Each collection is sequentially written in the
 *	file along with its name and its data. Each data (represented by Protobuf messages) is
 *	written in the file with a unique identifier (a UUID) and its type (the Protobuf message's type
 *	name).
 *	File databases can be added to the ghost::DataManager after they are created or loaded with
 *	the methods specific to this class.
 *	
 *	Saving the file to the disk is NOT automatic! Please call the "save()" method to write the data.
 */
class DatabaseFile : public ghost::Database
{
public:
	virtual ~DatabaseFile() = default;
	/**
	 *	Loads all the files named in the "filenames" list in the "root" directory.
	 *	The files are parsed and the data is loaded. After this operation, this method
	 *	returns a map containing a ghost::Database object for each file name that was
	 *	successfully read.
	 *	@param root directory in which the files are located
	 *	@param filenames list of strings representing the names (without path) of the files to load.
	 *	@return a map of loaded ghost::DatabsaeFile objects with their file names as keys.
	 */
	static std::map<std::string, std::shared_ptr<ghost::DatabaseFile>> load(
	    const std::string& root, const std::list<std::string>& filenames);
	/**
	 *	Creates a new file. After this call, "open()" shall be called to ensure that the file is
	 *	openable.
	 *	@param filename a local path or full path to a new file to open
	 *	@return a ghost::DatabaseFile representing the newly created database file.
	 */
	static std::shared_ptr<ghost::DatabaseFile> create(const std::string& filename);
	/**
	 *	Writes the content of the ghost::DatabaseFile including its collections to the specified
	 *	file on the disk. The operation fails if the file already exists, unless the flag "overwrite"
	 *	is set to true. In this case, the old file is discarded and the content is rewritten.
	 *	@param overwrite set to true if the existing file should be rewritten.
	 *	@return true if the write operation succeeded, false otherwise.
	 */
	virtual bool save(bool overwrite) = 0;
};
} // namespace ghost

#endif // GHOST_DATABASEFILE_HPP
