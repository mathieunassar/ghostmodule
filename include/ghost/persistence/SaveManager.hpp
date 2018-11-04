#ifndef GHOST_SAVE_MANAGER_HPP
#define GHOST_SAVE_MANAGER_HPP

#include <string>
#include <list>
#include <memory>

#include "SaveData.hpp"

namespace ghost
{
	// feature: while saving, backup the previous data and delete backup only on success
	// feature: writes every x seconds, keep a rotating backup of the saves
	class SaveManager
	{
	public:
		static std::shared_ptr<SaveManager> create(const std::string& root);

		virtual ~SaveManager() = 0;

		// adds data to the map under the key with title "file", creates the entry if it does not exist
		virtual void addData(std::shared_ptr<SaveData> data, const std::string& file) = 0;
		
		// searches the map for data sets of the given name and removes them, returns true if at least one was removed
		virtual bool removeData(const std::string& dataName) = 0;
		
		// searches the map for all the data sets of the given name and returns them as a list
		virtual std::list<std::shared_ptr<SaveData>> getData(const std::string& dataName) const = 0;

		// looks for all the save files in the root and reads them
		virtual bool load() = 0;

		// writes the saved data on the disk. If overwrite is true, replaces all the current data
		virtual bool save(bool overwrite) = 0;
	};

	inline SaveManager::~SaveManager() {}
}

#endif // GHOST_SAVE_MANAGER_HPP
