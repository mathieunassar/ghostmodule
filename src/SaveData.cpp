#include "../include/ghost/persistence/SaveData.hpp"

using namespace ghost;

SaveData::SaveData(const std::string& name)
	: internal::SaveData(name)
{

}

bool SaveData::remove(size_t index)
{
	if (index >= _data.size())
		return false;

	_data.erase(_data.begin() + index);
	return true;
}

// gets the name of this data set
const std::string& SaveData::getName() const
{
	return _name;
}

// returns the size of this data set
size_t SaveData::size() const
{
	return _data.size();
}

internal::SaveData::SaveData(const std::string& name)
	: _name(name)
{

}

void internal::SaveData::setData(const std::vector<std::shared_ptr<google::protobuf::Any>>& data)
{
	_data = data;
}

const std::vector<std::shared_ptr<google::protobuf::Any>>& internal::SaveData::getData() const
{
	return _data;
}
