#ifndef GHOST_SAVE_MANAGER_HPP
#define GHOST_SAVE_MANAGER_HPP

#include <string>
#include <list>
#include <map>
#include <memory>

#include "SaveData.hpp"

namespace ghost
{
	namespace internal
	{
		// feature: while saving, backup the previous data and delete backup only on success
		// feature: writes every x seconds, keep a rotating backup of the saves
		class SaveManager
		{
		public:
			// constructor, root is the path where the save manager will read/write the saves
			SaveManager(const std::string& root);

			// adds data to the map under the key with title "file", creates the entry if it does not exist
			void addData(std::shared_ptr<SaveData> data, const std::string& file);
			// searches the map for data sets of the given name and removes them, returns true if at least one was removed
			bool removeData(const std::string& dataName);
			// searches the map for all the data sets of the given name and returns them as a list
			std::list<std::shared_ptr<SaveData>> getData(const std::string& dataName) const;

			// looks for all the save files in the root and reads them
			bool load();
			// writes the saved data on the disk. If overwrite is true, replaces all the current data
			bool save(bool overwrite);

		private:
			std::map<std::string, std::list<std::shared_ptr<SaveData>>> _saveData;
		};
	}
}

#endif // GHOST_SAVE_MANAGER_HPP
