#include <ghost/persistence/SaveData.hpp>
#include "SaveData.hpp"

ghost::SaveData::SaveData(const std::string& name)
	: _internal(std::make_unique<ghost::internal::SaveData>(name))
{

}

bool ghost::SaveData::remove(size_t index)
{
	if (index >= _internal->getData().size())
		return false;

	_internal->getData().erase(_internal->getData().begin() + index);
	return true;
}

// gets the name of this data set
const std::string& ghost::SaveData::getName() const
{
	return _internal->getName();
}

// returns the size of this data set
size_t ghost::SaveData::size() const
{
	return _internal->getData().size();
}

// Implementation of the internal class

ghost::internal::SaveData::SaveData(const std::string& name)
	: _name(name)
{

}

void ghost::internal::SaveData::setData(const std::vector<std::shared_ptr<google::protobuf::Any>>& data)
{
	_data = data;
}

std::vector<std::shared_ptr<google::protobuf::Any>>& ghost::internal::SaveData::getData()
{
	return _data;
}

const std::vector<std::shared_ptr<google::protobuf::Any>>& ghost::internal::SaveData::getData() const
{
	return _data;
}

const std::string& ghost::internal::SaveData::getName() const
{
	return _name;
}
