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

bool ProtobufConfiguration::isAttributeEmpty(const std::string& name) const
{
	for (int i = 0; i < _configuration.fields_size(); i++)
	{
		if (_configuration.fields(i).name() == name)
			return _configuration.fields(i).value().empty();
	}
	return true;
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
