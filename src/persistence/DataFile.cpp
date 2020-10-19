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

#include "DataFile.hpp"

#include <fcntl.h>

#include <iostream>
#ifdef _WIN32
#include <io.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace ghost::internal;

DataFile::DataFile(const std::string& filename) : _filename(filename)
{
}

DataFile::~DataFile()
{
	close();
}

bool DataFile::open(Mode mode, bool overwrite)
{
	close(); // close anything if it was open

	if (mode == DataFile::READ)
	{
#ifdef _WIN32
		int handle = _open(_filename.c_str(), _O_RDONLY | _O_BINARY);
#else
		int handle = ::open(_filename.c_str(), O_RDONLY);
#endif
		if (handle == -1)
		{
			return false;
		}
		_fileInputStream =
		    std::make_shared<google::protobuf::io::FileInputStream>(handle); // this will manage the handle
		_codedInputStream = std::make_shared<google::protobuf::io::CodedInputStream>(_fileInputStream.get());
	}
	else
	{
#ifdef _WIN32
		int fileExists = _open(_filename.c_str(), _O_WRONLY); // try to open it to know if the file exists
#else
		int fileExists = ::open(_filename.c_str(), O_WRONLY); // try to open it to know if the file exists
#endif
		int handle = -1;

		if (fileExists != -1)
		{
#if _WIN32
			_close(fileExists); // close the test handle
#else
			::close(fileExists);			      // close the test handle
#endif
		}

		if (fileExists == -1 || overwrite) // if the file does not exist or the user wants to override it,
						   // reopen it with the truncate flags
		{
#ifdef _WIN32
			handle =
			    _open(_filename.c_str(), _O_WRONLY | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
#else
			handle = ::open(_filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
#endif
		}

		if (handle == -1) // if the file could not be created or if the file exists but overwrite is false
		{
			return false;
		}
		_fileOutputStream =
		    std::make_shared<google::protobuf::io::FileOutputStream>(handle); // this will manage the handle
		_codedOutputStream = std::make_shared<google::protobuf::io::CodedOutputStream>(_fileOutputStream.get());
	}
	return true;
}

bool DataFile::close()
{
	bool success = true;
	if (_codedInputStream) // file was open for reading
	{
		_codedInputStream.reset();
		success = success && _fileInputStream->Close();
		_fileInputStream.reset();
	}
	else if (_codedOutputStream) // file was open for writing
	{
		_codedOutputStream.reset();
		success = success && _fileOutputStream->Close();
		if (!success) std::cout << _fileOutputStream->GetErrno() << std::endl;
		_fileOutputStream.reset();
	}
	return success;
}

// writes the list of data in a row in the file
bool DataFile::write(const std::list<std::shared_ptr<ghost::internal::DataCollectionFile>>& data)
{
	if (!_codedOutputStream) return false; // the file is not open for writing

	for (auto& d : data)
	{
		// write collection's name
		_codedOutputStream->WriteVarint32(d->getName().size());
		_codedOutputStream->WriteString(d->getName());

		// write collection's next ID
		_codedOutputStream->WriteVarint64(d->getNextId());

		const auto& data = d->getData();
		for (const auto& message : data)
		{
			// write protobuf data length and data
			_codedOutputStream->WriteVarint32(message.second.length());
			_codedOutputStream->WriteString(message.second);
			
			// write message id
			_codedOutputStream->WriteVarint64(message.first);
		}

		// this means the end of a data set!
		_codedOutputStream->WriteVarint32(0);
	}

	return true;
}

// parses the file and returns the list of data
bool DataFile::read(std::list<std::shared_ptr<ghost::internal::DataCollectionFile>>& data)
{
	if (!_codedInputStream) return false; // the file was not open for reading

	bool hasNext = true;
	std::string nextDataSetName = "";
	google::protobuf::uint64 nextDataSetNextId = 0;
	std::map<size_t, std::string> set;
	while (hasNext)
	{
		// Read the size of the next message
		google::protobuf::uint32 size;
		hasNext = _codedInputStream->ReadVarint32(&size);
		if (!hasNext) break; // there are no more messages to read

		if (nextDataSetName.empty())
		{
			// Read the name of this set
			bool readSuccess = _codedInputStream->ReadString(&nextDataSetName, size);
			if (!readSuccess) return false; // failed!!!

			// Read the next ID of this set
			readSuccess = _codedInputStream->ReadVarint64(&nextDataSetNextId);
			if (!readSuccess) return false; // failed!!!

			continue;
		}

		if (size == 0) // this is the end of a data set!
		{
			auto newData =
			    std::make_shared<ghost::internal::DataCollectionFile>(nextDataSetName, nextDataSetNextId);
			newData->setData(set);
			data.push_back(newData);
			set.clear();
			nextDataSetName.clear();
			continue;
		}

		std::string data;
		
		bool readSuccess = _codedInputStream->ReadString(&data, size);
		if (!readSuccess) return false; // failed!!!

		google::protobuf::uint64 id;
		readSuccess = _codedInputStream->ReadVarint64(&id);
		if (!readSuccess) return false; // failed!!!

		set[id] = data;
	}

	if (!set.empty() && !nextDataSetName.empty()) // add the last set if it is not empty
	{
		auto newData = std::make_shared<ghost::internal::DataCollectionFile>(nextDataSetName, nextDataSetNextId);
		newData->setData(set);
		data.push_back(newData);
	}

	return true;
}
