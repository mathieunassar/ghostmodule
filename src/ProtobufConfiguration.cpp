#include "../include/ghost/connection/internal/ProtobufConfiguration.hpp"

#include <google/protobuf/repeated_field.h>

using namespace ghost::internal;

ProtobufConfiguration::ProtobufConfiguration(const std::string& name)
{
	_configuration.set_name(name);
}

const std::string& ProtobufConfiguration::getConfigurationName() const
{
	return _configuration.name();
}

bool ProtobufConfiguration::hasAttribute(const std::string& name) const
{
	for (int i = 0; i < _configuration.fields_size(); i++)
	{
		if (_configuration.fields(i).name() == name)
			return true;
	}
	return false;
}

std::map<std::string, std::string> ProtobufConfiguration::getAttributes() const
{
	std::map<std::string, std::string> res;
	auto iterator(_configuration.fields());
	for (auto it = iterator.begin(); it < iterator.end(); ++it)
	{
		res[it->name()] = it->value();
	}
	return res;
}

bool ProtobufConfiguration::removeAttribute(const std::string& name)
{
	auto iterator(_configuration.mutable_fields());
	for (auto it = iterator->begin(); it < iterator->end(); ++it)
	{
		if (it->name() == name)
		{
			iterator->erase(it);
			return true;
		}
	}
	return false;
}

std::string ProtobufConfiguration::getAttribute(const std::string& name) const
{
	auto iterator(_configuration.fields());
	for (auto it = iterator.begin(); it < iterator.end(); ++it)
	{
		if (it->name() == name)
		{
			return it->value();
		}
	}
	return "";
}
