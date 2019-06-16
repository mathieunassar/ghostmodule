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

#ifndef GHOST_INTERNAL_SAVE_FILE_HPP
#define GHOST_INTERNAL_SAVE_FILE_HPP

#include <string>
#include <list>
#include <memory>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/any.pb.h>

#include <ghost/persistence/SaveData.hpp>

namespace ghost
{
	namespace internal
	{
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

			bool open(Mode mode, bool overwrite = true);
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

#endif // GHOST_INTERNAL_SAVE_FILE_HPP
