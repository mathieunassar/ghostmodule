#include "../include/ghost/persistence/internal/SaveFile.hpp"
#include <iostream>

using namespace ghost::internal;

SaveFile::SaveFile(const std::string& filename)
	: _filename(filename)
{

}

SaveFile::~SaveFile()
{
	close();
}

bool SaveFile::open(Mode mode)
{
	close(); // close anything if it was open

	if (mode == SaveFile::READ)
	{
		int handle = _open(_filename.c_str(), _O_RDONLY | _O_BINARY);
		if (handle == -1)
		{
			return false;
		}
		_fileInputStream = std::make_shared<google::protobuf::io::FileInputStream>(handle); // this will manage the handle
		_codedInputStream = std::make_shared<google::protobuf::io::CodedInputStream>(_fileInputStream.get());
	}
	else
	{
		int handle = _open(_filename.c_str(), _O_WRONLY | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
		if (handle == -1)
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
bool SaveFile::write(const std::list<std::shared_ptr<SaveData>>& data)
{
	if (!_codedOutputStream)
		return false; // the file is not open for writing

	for (const auto& d : data)
	{
		// write name
		_codedOutputStream->WriteLittleEndian32(d->getName().size());
		_codedOutputStream->WriteString(d->getName());

		auto dataVector = d->getData();
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
bool SaveFile::read(std::list<std::shared_ptr<SaveData>>& data)
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
			data.push_back(std::make_shared<SaveData>(nextDataSetName, set));
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
		data.push_back(std::make_shared<SaveData>(nextDataSetName, set));
	}
	
	return true;
}
