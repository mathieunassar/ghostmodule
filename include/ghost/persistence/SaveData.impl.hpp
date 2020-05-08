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

template <typename DataType>
bool SaveData::get(DataType& type, size_t index) const
{
	if (index >= size()) return false;

	return getData().at(index)->UnpackTo(&type);
}

template <typename DataType>
std::list<DataType> SaveData::get_if(const std::function<bool(const DataType&)>& filter) const
{
	std::list<DataType> result;

	for (size_t i = 0; i < size(); ++i)
	{
		auto data = DataType::default_instance();

		bool getResult = get(data, i);
		if (!getResult) continue;

		if (filter(data)) result.push_back(data);
	}

	return result;
}

template <typename DataType>
size_t SaveData::replace_if(const std::function<bool(DataType&)>& operation)
{
	size_t updatedCount = 0;
	for (size_t i = 0; i < size(); ++i)
	{
		auto data = DataType::default_instance();

		bool getResult = get(data, i);
		if (!getResult) continue;

		if (operation(data))
		{
			replace(data, i);
			++updatedCount;
		}
	}

	return updatedCount;
}

template <typename DataType>
size_t SaveData::remove_if(const std::function<bool(DataType&)>& filter)
{
	size_t removedCount = 0;
	std::list<size_t> indicesToRemove;

	for (size_t i = 0; i < size(); ++i)
	{
		auto data = DataType::default_instance();

		bool getResult = get(data, i);
		if (!getResult) continue;

		if (filter(data))
		{
			indicesToRemove.push_back(i);
			++removedCount;
		}
	}

	// Start from the end because the data size is reduced after each call to "remove"
	for (auto it = indicesToRemove.rbegin(); it != indicesToRemove.rend(); ++it) remove(*it);

	return removedCount;
}

// adds data to the data set
template <typename DataType>
void SaveData::put(const DataType& type)
{
	auto any = std::make_shared<google::protobuf::Any>();
	any->PackFrom(type);
	getAllData().push_back(any);
}

template <typename DataType>
bool SaveData::replace(const DataType& type, size_t index)
{
	if (index >= size()) return false;

	auto any = std::make_shared<google::protobuf::Any>();
	any->PackFrom(type);
	getAllData()[index] = any;

	return true;
}