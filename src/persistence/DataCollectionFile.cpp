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

#include "DataCollectionFile.hpp"

using namespace ghost::internal;

DataCollectionFile::DataCollectionFile(const std::string& name) : _name(name)
{
}

bool DataCollectionFile::remove(size_t index)
{
	if (index >= _data.size()) return false;

	_data.erase(_data.begin() + index);
	return true;
}

const std::string& DataCollectionFile::getName() const
{
	return _name;
}

size_t DataCollectionFile::size() const
{
	return _data.size();
}

std::vector<std::shared_ptr<google::protobuf::Any>>& DataCollectionFile::getData()
{
	return _data;
}

void DataCollectionFile::setData(const std::vector<std::shared_ptr<google::protobuf::Any>>& data)
{
	_data = data;
}

std::vector<std::shared_ptr<google::protobuf::Any>> DataCollectionFile::fetch(const std::string& typeName)
{
	if (typeName.empty()) return _data;

	std::vector<std::shared_ptr<google::protobuf::Any>> found;
	for (const auto& d : _data)
	{
		std::string trueType = getTrueTypeName(*d);
		if (trueType == typeName) found.push_back(d);
	}
	return found;
}

bool DataCollectionFile::push(const std::shared_ptr<google::protobuf::Any>& data, int index)
{
	if (index < 0)
		_data.push_back(data);
	else
	{
		if (index >= size()) return false;
		_data[index] = data;
	}
	return true;
}

std::string DataCollectionFile::getTrueTypeName(const google::protobuf::Any& message)
{
	const std::string& url = message.type_url();
	size_t last = url.find_last_of('/');
	if (last == std::string::npos)
	{
		return url;
	}
	return url.substr(last + 1);
}
