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

#ifndef GHOST_SAVE_MANAGER_HPP
#define GHOST_SAVE_MANAGER_HPP

#include <ghost/persistence/SaveData.hpp>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace ghost
{
/**
 * @brief The ghost::SaveManager is responsible for the storage and the loading of ghost::SaveData objects.
 *
 * ghost::SaveData objects are managed by their name and the target file, which they shall be stored to.
 * It is possible to manage multiple ghost::SaveData objects with the same name.
 * Management methods are provided to add, remove and retrieve data from the manager.
 *
 * In order to trigger disk writing or reading operations, the methods "load" and "save" are provided.
 * See their documentation for details about their function.
 */
class SaveManager
{
public:
	/**
	 * @brief Creates a save manager.
	 *
	 * @param root the folder in which the save files are stored
	 * @return a new save manager
	 */
	static std::shared_ptr<SaveManager> create(const std::string& root);

	virtual ~SaveManager() = default;

	/**
	 * @brief Adds data in the manager, which will be stored in the file with the provided name on the
	 * next call to "save".
	 *
	 * If this is the first time data is added for this file name, a new file entry will be created.
	 * Storing the data will happen during a call to "save".
	 *
	 * If "file" is left empty, the data will be mapped to a file entry named using the following template:
	 * [data->getName()] + ".dat".
	 *
	 * @param data data to manage
	 * @param file destination file in which this data will be stored.
	 */
	virtual void addData(std::shared_ptr<ghost::SaveData> data, std::string file = "") = 0;

	/**
	 * @brief Removes all managed ghost::SaveData objects in this manager with the given name and returns true
	 * if at least one instance was removed.
	 *
	 * If filename is provided, only the ghost::SaveData objects will be removed, that have the provided file name
	 * as destination storage file. Leave empty to remove ghost::SaveData objects with the matching name for all
	 * file names.
	 *
	 * If prunEmptyFiles is set to true, files that have no more managed saveData objects will no longer be managed
	 * by the manager. Otherwise, during a call to "save", empty files will be re-written empty.
	 *
	 * @param dataName name of the data to remove
	 * @param filename if not empty, ghost::SaveData objects to remove will be only searches for this filename.
	 * @param pruneEmptyFiles If set to true, files with no associated data will not be managed anymore
	 * @return true if data was removed
	 * @return false if no data was removed during this call
	 */
	virtual bool removeData(const std::string& dataName, const std::string& filename = "",
				bool pruneEmptyFiles = false) = 0;

	/**
	 * @brief Searches the manager for all ghost::SaveData objects with the provided name and returns them as a map.
	 * The key used by the returned map corresponds to the file names used for the ghost::SavedData as a destination
	 * storage file.
	 *
	 * @param dataName name of the data sets to search
	 * @return  a map containing the destination files as a key and a list of the matching SaveData objects
	 */
	virtual std::map<std::string, std::list<std::shared_ptr<ghost::SaveData>>> getData(
	    const std::string& dataName) const = 0;

	/**
	 * @brief Empties the data managed by this manager and loads the data from the provided files.
	 *
	 * Warning: all data previously added to this manager will be deleted and replaced by the data in the
	 * provided files!
	 *
	 * If an error occurs during the loading process, this methods will return false and the data read will be
	 * deleted. For example, if one of the files provided in the parameter list does not exist or is corrupted, this
	 * method will fail.
	 *
	 * Upon success, the manager contains SaveData objects which can be accessed by the user.
	 *
	 * @param files list of files contianing the SaveData objects to be read.
	 * @return true If the loading operation succeeded.
	 * @return false If an error occurred while reading the files.
	 */
	virtual bool load(const std::list<std::string>& files) = 0;

	// writes the saved data on the disk. If overwrite is true, replaces all the current data
	/**
	 * @brief Dumps all the managed data onto the disk.
	 *
	 * Stored SaveData objects will be written in the files described by the names they were associated to.
	 * If a file does not exist, then it is created.
	 * If a file already exists and the flag "overwrite" is set to true, then the existing file is truncated.
	 * If the file already exists and the flag "overwrite" is set to false, then the save process fails, possibly
	 * leading to an inconsistent state if the manager manages multiple files, and if this error occurs after one
	 * or more files were already written.
	 *
	 * @param overwrite set to true to replace the configured files if they were already existing
	 * @return true if the save process succeeeds
	 * @return false if an error occurs while writing to the files, or if one of the configured files already exists
	 * and the "overwrite" flag is set to false.
	 */
	virtual bool save(bool overwrite) = 0;

	/**
	 * @brief Makes a list of all the file names that are currently managed by the manager.
	 *
	 * During the save process, files matching the returned names will be written, even if there is no data
	 * associated to them.
	 *
	 * @return a list of all the files managed by this object.
	 */
	virtual std::list<std::string> getFileNames() const = 0;
};
} // namespace ghost

#endif // GHOST_SAVE_MANAGER_HPP
