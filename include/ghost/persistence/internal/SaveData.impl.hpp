template<typename DataType>
bool SaveData::get(DataType& type, size_t index) const
{
	if (index >= _data.size())
		return false;

	return _data.at(index)->UnpackTo(&type);
}

// adds data to the data set
template<typename DataType>
void SaveData::put(const DataType& type)
{
	auto any = std::make_shared<google::protobuf::Any>();
	any->PackFrom(type);
	_data.push_back(any);
}

template<typename DataType>
bool SaveData::replace(const DataType& type, size_t index)
{
	if (index >= _data.size())
		return false;

	auto any = std::make_shared<google::protobuf::Any>();
	any->PackFrom(type);
	_data[index] = any;

	return true;
}