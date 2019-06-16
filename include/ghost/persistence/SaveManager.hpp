#ifndef GHOST_SAVE_MANAGER_HPP
#define GHOST_SAVE_MANAGER_HPP

#include <string>
#include <list>
#include <map>
#include <memory>

#include "SaveData.hpp"

namespace ghost
{
	/**
	 * @brief The SaveManager is responsible for the storage and the loading of SaveData objects.
	 * 
	 * SaveData objects are managed by their name and the target file, which they shall be stored to.
	 * It is possible to manage multiple SaveData objects with the same name.
	 * Management methods are provided to add, remove and retrieve data from the manager.
	 * 
	 * In order to trigger disk writing or reading operations, the methods "load" and "save are provided".
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

		virtual ~SaveManager() = 0;

		/**
		 * @brief Adds data in the manager, which will be stored in the file with the provided name.
		 * 
		 * If this is the first time data is added for this file name, a new file entry will be created.
		 * Storing the data will happen during a call to load.
		 * 
		 * @param data data to manage
		 * @param file destination file in which this data will be stored.
		 */
		virtual void addData(std::shared_ptr<ghost::SaveData> data, const std::string& file) = 0;
		
		/**
		 * @brief Removes all managed SaveData objects in this manager and returns true if at least one instance was removed.
		 * 
		 * If filename is provided, only the SaveData will be removed, that have the provided file name as destination
		 * storage file. Leave empty to remove SaveData objects with the matching name for all file names.
		 * 
		 * If prunEmptyFiles is set to true, files that have no more managed saveData objects will no longer be managed by
		 * the manager. Otherwise, during a call to "save", empty files will be re-written empty.
		 * 
		 * @param dataName name of the data to remove
		 * @param filename if not empty, SaveData objects to remove will be only searches for this filename.
		 * @param pruneEmptyFiles If set to true, files with no associated data will not be managed anymore
		 * @return true if data was removed
		 * @return false if no data was removed during this call
		 */
		virtual bool removeData(const std::string& dataName, const std::string& filename = "", bool pruneEmptyFiles = false) = 0;
		
		/**
		 * @brief Searches the manager for all SaveData objects with the provided name and returns them as a map. The key
		 * used by the returned map corresponds to the file names used the the SavedData as a destination storage file.
		 * 
		 * @param dataName name of the data sets to search
		 * @return  a map containing the destination file as a key and a list of the matching SaveData objects
		 */
		virtual std::map<std::string, std::list<std::shared_ptr<ghost::SaveData>>> getData(const std::string& dataName) const = 0;

		/**
		 * @brief Empties the data managed by this manager and loads the data from the provided files.
		 * 
		 * Warning: all data previously added to this manager will be deleted and replaced by the data in the
		 * provided files!
		 * 
		 * If an error occurs during the loading process, this methods will return false and the data read will be deleted.
		 * For example, if one of the files provided in the parameter list does not exist or is corrupted, this method will fail.
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
		 * If the file already exists and the flag "overwrite" is set to false, the the save process fails, possibly
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
		 * @brief Makes a list of all the file names that are managed by the manager.
		 * 
		 * During the save process, files matching the returned names will be written, even if there is no data
		 * associated to them.
		 * 
		 * @return a list of all the files managed by this object. 
		 */
		virtual std::list<std::string> getFileNames() const = 0;
	};

	inline SaveManager::~SaveManager() {}
}

#endif // GHOST_SAVE_MANAGER_HPP
