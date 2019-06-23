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

template<typename DataType>
bool SaveData::get(DataType& type, size_t index) const
{
	if (index >= size())
		return false;

	return getData().at(index)->UnpackTo(&type);
}

// adds data to the data set
template<typename DataType>
void SaveData::put(const DataType& type)
{
	auto any = std::make_shared<google::protobuf::Any>();
	any->PackFrom(type);
	getData().push_back(any);
}

template<typename DataType>
bool SaveData::replace(const DataType& type, size_t index)
{
	if (index >= size())
		return false;

	auto any = std::make_shared<google::protobuf::Any>();
	any->PackFrom(type);
	getData()[index] = any;

	return true;
}