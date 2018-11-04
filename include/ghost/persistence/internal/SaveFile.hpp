#ifndef GHOST_SAVE_FILE_HPP
#define GHOST_SAVE_FILE_HPP

#include <string>
#include <list>
#include <memory>
#include <fcntl.h>  
#include <io.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/any.pb.h>

#include "../SaveData.hpp"

namespace ghost
{
	namespace internal
	{
		/*
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
