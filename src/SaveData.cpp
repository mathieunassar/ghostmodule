#include "../include/ghost/persistence/internal/SaveData.hpp"

using namespace ghost;

SaveData::SaveData(const std::string& name)
	: _name(name)
{

}

internal::SaveData::SaveData(const std::string& name, const std::vector<std::shared_ptr<google::protobuf::Any>>& data)
	: ghost::SaveData(name)
{
	_data = data;
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

const std::vector<std::shared_ptr<google::protobuf::Any>>& internal::SaveData::getData() const
{
	return _data;
}
