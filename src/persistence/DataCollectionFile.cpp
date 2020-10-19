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
#include <google/protobuf/util/json_util.h>

using namespace ghost::internal;

DataCollectionFile::DataCollectionFile(const std::string& name, size_t nextId) : _name(name), _nextId(nextId)
{
}

bool DataCollectionFile::remove(size_t id)
{
	return _data.erase(id);
}

const std::string& DataCollectionFile::getName() const
{
	return _name;
}

size_t DataCollectionFile::getNextId() const
{
	return _nextId;
}

size_t DataCollectionFile::size() const
{
	return _data.size();
}

std::map<size_t, std::string>& DataCollectionFile::getData()
{
	return _data;
}

void DataCollectionFile::setData(const std::map<size_t, std::string>& data)
{
	_data = data;
}

std::map<size_t, std::shared_ptr<google::protobuf::Message>> DataCollectionFile::fetch(
    const std::function<std::shared_ptr<google::protobuf::Message>()>& messageFactory, std::list<size_t> idFilter)
{
	std::map<size_t, std::shared_ptr<google::protobuf::Message>> result;

	if (idFilter.empty())
	{
		for (const auto& entry : _data)
		{
			auto msg = messageFactory();
			google::protobuf::Any any;
			any.ParseFromString(entry.second);
			if (getTrueTypeName(any) == msg->GetTypeName() && msg->ParseFromString(any.value()))
				result[entry.first] = msg;
		}
	}
	else
	{
		for (const auto& id : idFilter)
		{
			if (_data.find(id) == _data.end()) continue;

			auto msg = messageFactory();

			google::protobuf::Any any;
			any.ParseFromString(_data.at(id));
			if (getTrueTypeName(any) == msg->GetTypeName() && msg->ParseFromString(any.value()))
				result[id] = msg;
		}
	}

	return result;
}

bool DataCollectionFile::push(const google::protobuf::Message& data, size_t id)
{
	google::protobuf::Any any;
	any.PackFrom(data);
	if (_data.find(id) == _data.end())
	{
		_data[_nextId] = any.SerializeAsString();
		++_nextId;
	}
	else
		_data[id] = any.SerializeAsString();
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
