#ifndef GHOST_SAVE_FILE_HPP
#define GHOST_SAVE_FILE_HPP

#include <string>
#include <list>
#include <vector>
#include <map>
#include <memory>

#include <fcntl.h>  
#include <io.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <google/protobuf/any.pb.h>

namespace ghost
{
	class SaveData
	{
	public:
		SaveData(const std::vector<std::shared_ptr<google::protobuf::Any>>& data);

		// reads data at a specific index, or returns false if there is no such index or if the conversion fails
		template<typename DataType>
		bool get(DataType& type, size_t index) const;

		// adds data to the data set
		template<typename DataType>
		void put(const DataType& type);

		// gets the name of this data set
		const std::string& getName() const;

		// returns the size of this data set
		size_t size() const;

		const std::vector<std::shared_ptr<google::protobuf::Any>>& getData() const;

	private:
		std::vector<std::shared_ptr<google::protobuf::Any>> _data;
		std::string _name;
	};

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

	namespace internal
	{
		/*
		// TODO: encode save data name and read it back
		how this will work:
		- gets the list of the any messages in the data set;
		- write them
		- reading read the message as any messages
		- user gets the messages with templated calls that convert the messages
		- possibility to use ghost::Message - specialize put and get of SaveData to convert ghost::Message to and from Any.
		- could even be split into ReadSaveFile and WriteSaveFile but mäh
		*/
		class SaveFile
		{
		public:
			enum Mode
			{
				READ,
				WRITE
			};

			SaveFile(const std::string& filename);
			~SaveFile();

			bool open(Mode mode);
			bool close();

			// writes the list of data in a row in the file
			bool write(const std::list<std::shared_ptr<ghost::SaveData>>& data);
			// parses the file and returns the list of data
			bool read(std::list<std::shared_ptr<ghost::SaveData>>& data);

		private:
			std::string _filename;

			// for input
			std::shared_ptr<google::protobuf::io::FileInputStream> _fileInputStream;
			std::shared_ptr<google::protobuf::io::CodedInputStream> _codedInputStream;

			// for output
			std::shared_ptr<google::protobuf::io::FileOutputStream> _fileOutputStream;
			std::shared_ptr<google::protobuf::io::CodedOutputStream> _codedOutputStream;
		};
	}
}

#endif // GHOST_SAVE_FILE_HPP
