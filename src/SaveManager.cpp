#include "../include/ghost/persistence/internal/SaveManager.hpp"

using namespace ghost::internal;

std::shared_ptr<ghost::SaveManager> ghost::SaveManager::create(const std::string& root)
{
	return std::make_shared<ghost::internal::SaveManager>(root);
}

// constructor, root is the path where the save manager will read/write the saves
SaveManager::SaveManager(const std::string& root)
{

}

// adds data to the map under the key with title "file", creates the entry if it does not exist
void SaveManager::addData(std::shared_ptr<ghost::SaveData> data, const std::string& file)
{

}

// searches the map for data sets of the given name and removes them, returns true if at least one was removed
bool SaveManager::removeData(const std::string& dataName)
{
	return false;
}

// searches the map for all the data sets of the given name and returns them as a list
std::list<std::shared_ptr<ghost::SaveData>> SaveManager::getData(const std::string& dataName) const
{
	return {};
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
