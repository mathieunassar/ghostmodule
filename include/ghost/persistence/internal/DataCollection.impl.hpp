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
bool DataCollection::get(DataType& type, size_t id)
{
	auto search = fetch([]() { return std::make_shared<DataType>(); }, {id});
	if (search.size() == 0) return false;

	type = *(std::static_pointer_cast<DataType>(search.at(id)));
	return true;
}

template <typename DataType>
std::map<size_t, DataType> DataCollection::get_if(const std::function<bool(const DataType&, size_t id)>& filter)
{
	std::map<size_t, DataType> result;

	auto matchingMessages = fetch([]() { return std::make_shared<DataType>(); });
	for (const auto& message : matchingMessages)
	{
		DataType msg = *(std::static_pointer_cast<DataType>(message.second));
		if (filter(msg, message.first)) result[message.first] = std::move(msg);
	}

	return result;
}

// adds data to the data set
template <typename DataType>
void DataCollection::put(const DataType& type)
{
	push(type);
}

template <typename DataType>
bool DataCollection::replace(const DataType& type, size_t id)
{
	return push(type, id);
}

template <typename DataType>
size_t DataCollection::replace_if(const std::function<bool(DataType&, size_t id)>& operation)
{
	size_t updatedCount = 0;
	auto matchingMessages = fetch([]() { return std::make_shared<DataType>(); });
	for (const auto& message : matchingMessages)
	{
		DataType msg = *(std::static_pointer_cast<DataType>(message.second));
		if (operation(msg, message.first))
		{
			replace(msg, message.first);
			++updatedCount;
		}
	}

	return updatedCount;
}

template <typename DataType>
size_t DataCollection::remove_if(const std::function<bool(DataType&, size_t id)>& filter)
{
	size_t removedCount = 0;

	auto matchingMessages = fetch([]() { return std::make_shared<DataType>(); });
	for (const auto& message : matchingMessages)
	{
		DataType msg = *(std::static_pointer_cast<DataType>(message.second));
		if (filter(msg, message.first))
		{
			remove(message.first);
			++removedCount;
		}
	}

	return removedCount;
}

} // namespace ghost