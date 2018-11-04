#include "../include/ghost/persistence/internal/SaveManager.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::SaveManager> ghost::SaveManager::create(const std::string& root)
{
	return std::make_shared<ghost::internal::SaveManager>(root);
}

// constructor, root is the path where the save manager will read/write the saves
SaveManager::SaveManager(const std::string& root)
	: _saveRoot(root)
{

}

// adds data to the map under the key with title "file", creates the entry if it does not exist
void SaveManager::addData(std::shared_ptr<ghost::SaveData> data, const std::string& file)
{
	if (_saveData.count(file) == 0)
		_saveData[file] = std::list<std::shared_ptr<SaveData>>();

	_saveData[file].push_back(data);
}

// searches the map for data sets of the given name and removes them, returns true if at least one was removed
bool SaveManager::removeData(const std::string& dataName, const std::string& filename)
{
	bool removedSome = false;

	auto it = _saveData.begin();
	while (it != _saveData.end())
	{
		// if filename is not empty, the user wants to remove a data set from a specific file ...
		if (!filename.empty() && it->first != filename) // ... which is not this one -> continue
		{
			++it;
			continue;
		}

		auto it2 = it->second.begin();
		while (it2 != it->second.end())
		{
			if ((*it2)->getName() == dataName)
			{
				it2 = it->second.erase(it2);
				removedSome = true;
			}
			else
				++it2;
		}

		if (it->second.empty())
			it = _saveData.erase(it);
		else
			++it;
	}
	return removedSome;
}

// searches the map for all the data sets of the given name and returns them as a list
std::map<std::string, std::list<std::shared_ptr<ghost::SaveData>>> SaveManager::getData(const std::string& dataName) const
{
	std::map<std::string, std::list<std::shared_ptr<ghost::SaveData>>> res;
	for (const auto& entry : _saveData)
	{
		std::list<std::shared_ptr<ghost::SaveData>> found;
		for (const auto& data : entry.second)
		{
			if (data->getName() == dataName)
				found.push_back(data);
		}
		if (!found.empty())
			res[entry.first] = found;
	}

	return res;
}

// looks for all the save files in the root and reads them
bool SaveManager::load()
{
	return false;
}

// writes the saved data on the disk. If overwrite is true, replaces all the current data
bool SaveManager::save(bool overwrite)
{
	return false;
}
