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

#include "SaveData.hpp"

#include <ghost/persistence/SaveData.hpp>

std::shared_ptr<ghost::SaveData> ghost::SaveData::create(const std::string& name)
{
	return std::make_shared<ghost::internal::SaveData>(name);
}

// Implementation of the internal class

ghost::internal::SaveData::SaveData(const std::string& name) : _name(name)
{
}

bool ghost::internal::SaveData::remove(size_t index)
{
	if (index >= _data.size()) return false;

	_data.erase(_data.begin() + index);
	return true;
}

const std::string& ghost::internal::SaveData::getName() const
{
	return _name;
}

size_t ghost::internal::SaveData::size() const
{
	return _data.size();
}

const std::vector<std::shared_ptr<google::protobuf::Any>>& ghost::internal::SaveData::getData() const
{
	return _data;
}

std::vector<std::shared_ptr<google::protobuf::Any>>& ghost::internal::SaveData::getData()
{
	return _data;
}

void ghost::internal::SaveData::setData(const std::vector<std::shared_ptr<google::protobuf::Any>>& data)
{
	_data = data;
}
