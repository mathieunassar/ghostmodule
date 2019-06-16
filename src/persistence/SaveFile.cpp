#include "../include/ghost/persistence/internal/SaveFile.hpp"
#include <iostream>
#include <fcntl.h>
#ifdef _WIN32
	#include <io.h>
#else
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>
#endif

using namespace ghost::internal;

SaveFile::SaveFile(const std::string& filename)
	: _filename(filename)
{

}

SaveFile::~SaveFile()
{
	close();
}

bool SaveFile::open(Mode mode, bool overwrite)
{
	close(); // close anything if it was open

	if (mode == SaveFile::READ)
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
		_fileInputStream = std::make_shared<google::protobuf::io::FileInputStream>(handle); // this will manage the handle
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
			::close(fileExists); // close the test handle
#endif
		}


		if (fileExists == -1 || overwrite) // if the file does not exist or the user wants to override it, reopen it with the truncate flags
		{
#ifdef _WIN32
			handle = _open(_filename.c_str(), _O_WRONLY | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
#else
			handle = ::open(_filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
#endif
		}

		if (handle == -1) // if the file could not be created or if the file exists but overwrite is false
		{
			return false;
		}
		_fileOutputStream = std::make_shared<google::protobuf::io::FileOutputStream>(handle); // this will manage the handle
		_codedOutputStream = std::make_shared<google::protobuf::io::CodedOutputStream>(_fileOutputStream.get());
	}
	return true;
}

bool SaveFile::close()
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
		if (!success)
			std::cout << _fileOutputStream->GetErrno() << std::endl;
		_fileOutputStream.reset();
	}
	return success;
}

// writes the list of data in a row in the file
bool SaveFile::write(const std::list<std::shared_ptr<ghost::SaveData>>& data)
{
	if (!_codedOutputStream)
		return false; // the file is not open for writing

	for (const auto& d : data)
	{
		// write name
		_codedOutputStream->WriteLittleEndian32(d->getName().size());
		_codedOutputStream->WriteString(d->getName());

		auto dataVector = ((internal::SaveData*)d.get())->getData(); // C-style cast can get over the private inheritance used to hide impl detail to the user
		for (const auto& message : dataVector)
		{
			_codedOutputStream->WriteLittleEndian32(message->ByteSize());
			bool serializationSuccess = message->SerializeToCodedStream(_codedOutputStream.get());

			if (!serializationSuccess)
				return false; // failed!!!
		}

		// this means the end of a data set!
		_codedOutputStream->WriteLittleEndian32(0);
	}

	return true;
}

// parses the file and returns the list of data
bool SaveFile::read(std::list<std::shared_ptr<ghost::SaveData>>& data)
{
	if (!_codedInputStream)
		return false; // the file was not open for reading
	
	bool hasNext = true;
	std::string nextDataSetName = "";
	std::vector<std::shared_ptr<google::protobuf::Any>> set;
	while (hasNext)
	{
		google::protobuf::uint32 size;
		hasNext = _codedInputStream->ReadLittleEndian32(&size); // read the size of the next message
		if (!hasNext)
			break; // there are no more messages to read

		if (nextDataSetName.empty())
		{
			bool readSuccess = _codedInputStream->ReadString(&nextDataSetName, size);
			if (!readSuccess)
				return false; // failed!!!
			continue;
		}

		if (size == 0) // this is the end of a data set!
		{
			auto newData = std::make_shared<ghost::SaveData>(nextDataSetName);
			((internal::SaveData*)newData.get())->setData(set); // C-style cast can get over the private inheritance used to hide impl detail to the user
			data.push_back(newData);
			set.clear();
			nextDataSetName.clear();
			continue;
		}

		auto message = std::make_shared<google::protobuf::Any>();

		google::protobuf::io::CodedInputStream::Limit msgLimit = _codedInputStream->PushLimit(size);
		bool readSuccess = message->ParseFromCodedStream(_codedInputStream.get());
		if (!readSuccess)
			return false; // failed!!!

		set.push_back(message);
		_codedInputStream->PopLimit(msgLimit);
	}

	if (!set.empty() && !nextDataSetName.empty()) // add the last set if it is not empty
	{
		auto newData = std::make_shared<ghost::SaveData>(nextDataSetName);
		((internal::SaveData*)newData.get())->setData(set); // C-style cast can get over the private inheritance used to hide impl detail to the user
		data.push_back(newData);
	}
	
	return true;
}
