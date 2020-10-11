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

namespace ghost
{
template <typename DataType>
bool DataCollection::get(DataType& type, size_t index)
{
	if (index >= size()) return false;

	return fetch().at(index)->UnpackTo(&type);
}

template <typename DataType>
std::list<DataType> DataCollection::get_if(const std::function<bool(const DataType&)>& filter)
{
	std::list<DataType> result;

	auto matchingMessages = fetch(DataType::default_instance().GetTypeName());
	for (const auto& message : matchingMessages)
	{
		auto data = DataType::default_instance();

		bool getResult = message->UnpackTo(&data);
		if (!getResult) continue;

		if (filter(data)) result.push_back(data);
	}

	return result;
}

// adds data to the data set
template <typename DataType>
void DataCollection::put(const DataType& type)
{
	auto any = std::make_shared<google::protobuf::Any>();
	any->PackFrom(type);
	push(any);
}

template <typename DataType>
bool DataCollection::replace(const DataType& type, size_t index)
{
	auto any = std::make_shared<google::protobuf::Any>();
	any->PackFrom(type);
	return push(any, index);
}

template <typename DataType>
size_t DataCollection::replace_if(const std::function<bool(DataType&)>& operation)
{
	size_t updatedCount = 0;
	auto matchingMessages = fetch();
	for (size_t i = 0; i < matchingMessages.size(); ++i)
	{
		auto data = DataType::default_instance();

		bool getResult = matchingMessages[i]->UnpackTo(&data);
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
size_t DataCollection::remove_if(const std::function<bool(DataType&)>& filter)
{
	size_t removedCount = 0;
	std::list<size_t> indicesToRemove;

	auto matchingMessages = fetch();
	for (size_t i = 0; i < matchingMessages.size(); ++i)
	{
		auto data = DataType::default_instance();

		bool getResult = matchingMessages[i]->UnpackTo(&data);
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

} // namespace ghost