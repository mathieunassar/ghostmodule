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

#ifndef GHOST_INTERNAL_SAVE_MANAGER_HPP
#define GHOST_INTERNAL_SAVE_MANAGER_HPP

#include <string>
#include <list>
#include <map>
#include <memory>

#include <ghost/persistence/SaveManager.hpp>

namespace ghost
{
	namespace internal
	{
		// possible feature: while saving, backup the previous data and delete backup only on success
		// possible feature: writes every x seconds, keep a rotating backup of the saves
		// possible feature: possibility to encrypt the files.
		class SaveManager : public ghost::SaveManager
		{
		public:
			// constructor, root is the path where the save manager will read/write the saves
			SaveManager(const std::string& root);

			// adds data to the map under the key with title "file", creates the entry if it does not exist
			void addData(std::shared_ptr<ghost::SaveData> data, const std::string& file) override;
			
			// searches the map for data sets of the given name and removes them, returns true if at least one was removed
			bool removeData(const std::string& dataName, const std::string& filename = "", bool pruneEmptyFiles = false) override;
			
			// searches the map for all the data sets of the given name and returns them as a list
			std::map<std::string, std::list<std::shared_ptr<ghost::SaveData>>> getData(const std::string& dataName) const override;

			// looks for all the save files in the root and reads them
			bool load(const std::list<std::string>& files) override;

			// writes the saved data on the disk. If overwrite is true, replaces all the current data
			bool save(bool overwrite) override;

			std::list<std::string> getFileNames() const override;

		private:
			/**
				This map contains the save data as follows:
				KEY = name of the file in which the data will be stored
				VALUE = data, also containing its name

				_Note_: it is possible to have multiple data with the same name
			*/
			std::map<std::string, std::list<std::shared_ptr<ghost::SaveData>>> _saveData;
			std::string _saveRoot;
		};
	}
}

#endif // GHOST_INTERNAL_SAVE_MANAGER_HPP
