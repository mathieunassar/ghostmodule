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

#include "DatabaseFile.hpp"
#include "DataFile.hpp"

using namespace ghost::internal;

std::map<std::string, std::shared_ptr<ghost::DatabaseFile>> ghost::DatabaseFile::load(
    const std::string& root, const std::list<std::string>& filenames)
{
	std::map<std::string, std::shared_ptr<ghost::DatabaseFile>> databases;
	for (const auto& file : filenames)
	{
		std::string fullPath = root + "/" + file;
		auto database = std::make_shared<ghost::internal::DatabaseFile>(fullPath);
		if (database->open()) databases[file] = (database);
	}
	return databases;
}

std::shared_ptr<ghost::DatabaseFile> ghost::DatabaseFile::create(const std::string& filename)
{
	return std::make_shared<ghost::internal::DatabaseFile>(filename);
}

DatabaseFile::DatabaseFile(const std::string& filename) : _filename(filename)
{
}

bool DatabaseFile::open()
{
	DataFile file(_filename);

	bool openSuccess = file.open(DataFile::READ);
	if (!openSuccess)
	{
		_data.clear();
		return false;
	}

	std::list<std::shared_ptr<ghost::internal::DataCollectionFile>> data;
	bool readSuccess = file.read(data);
	if (!readSuccess)
	{
		_data.clear();
		return false;
	}

	file.close();
	_data = data;
	return true;
}

void DatabaseFile::close()
{
	// Nothing to do
}

std::shared_ptr<ghost::DataCollection> DatabaseFile::addCollection(const std::string& name)
{
	auto newCollection = std::make_shared<DataCollectionFile>(name);
	_data.push_back(newCollection);
	return newCollection;
}

bool DatabaseFile::removeCollections(const std::string& name)
{
	bool removedSome = false;
	auto it = _data.begin();
	while (it != _data.end())
	{
		if ((*it)->getName() == name)
		{
			it = _data.erase(it);
			removedSome = true;
		}
		else
			++it;
	}
	return removedSome;
}

std::list<std::shared_ptr<ghost::DataCollection>> DatabaseFile::getCollections(const std::string& name) const
{
	std::list<std::shared_ptr<ghost::DataCollection>> found;
	for (const auto& data : _data)
	{
		if (data->getName() == name) found.push_back(data);
	}
	return found;
}

bool DatabaseFile::save(bool overwrite)
{
	DataFile file(_filename);
	bool openSuccess = file.open(DataFile::WRITE, overwrite);
	if (!openSuccess)
	{
		// TODO restore backup
		return false;
	}

	bool writeSuccess = file.write(_data);
	if (!writeSuccess)
	{
		// TODO restore backup
		return false;
	}

	file.close();
	return true;
}
