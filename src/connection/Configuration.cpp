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

#include "Configuration.hpp"

using namespace ghost::internal;

// Implementation of Configuration API

ghost::ConfigurationValue::ConfigurationValue(const std::string& stringValue)
	: _value(stringValue)
{

}

const std::string& ghost::ConfigurationValue::getValue() const
{
	return _value;
}

std::shared_ptr<ghost::Configuration> ghost::Configuration::create(const std::string& name)
{
	return std::make_shared<ghost::internal::Configuration>(name);
}

const std::string ghost::Configuration::EMPTY = "";

// Implementation of Configuration internals

Configuration::Configuration(const std::string& name)
{
	_configuration.set_name(name);
}

void Configuration::copy(ghost::Configuration& to) const
{
	ghost::internal::Configuration& config = dynamic_cast<ghost::internal::Configuration&>(to);
	config._configuration = _configuration;
}

const std::string& Configuration::getConfigurationName() const
{
	return _configuration.name();
}

bool Configuration::hasAttribute(const std::string& name) const
{
	for (int i = 0; i < _configuration.fields_size(); i++)
	{
		if (_configuration.fields(i).name() == name)
			return true;
	}
	return false;
}

bool Configuration::isAttributeEmpty(const std::string& name) const
{
	for (int i = 0; i < _configuration.fields_size(); i++)
	{
		if (_configuration.fields(i).name() == name)
			return _configuration.fields(i).value().empty();
	}
	return true;
}

bool Configuration::getAttribute(const std::string& name, ghost::ConfigurationValue& value) const
{
	if (!hasAttribute(name))
		return false;

	value = ghost::ConfigurationValue(getAttribute(name));
	return true;
}

bool Configuration::getAttribute(const std::string& name, ghost::ConfigurationValue& value, const ghost::ConfigurationValue& defaultValue) const
{
	if (!hasAttribute(name))
	{
		value = defaultValue;
		return false;
	}

	value = ghost::ConfigurationValue(getAttribute(name));
	return true;
}

std::map<std::string, std::string> Configuration::getAttributes() const
{
	std::map<std::string, std::string> res;
	for (const ghost::protobuf::connection::ConnectionConfigurationField& str : _configuration.fields())
	{
		res[str.name()] = str.value();
	}
	return res;
}

bool Configuration::removeAttribute(const std::string& name)
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

bool Configuration::addAttribute(const std::string& name, const ghost::ConfigurationValue& value, bool overwrite)
{
	if (hasAttribute(name))
	{
		if (!overwrite)
			return false;

		return updateAttribute(name, value);
	}

	protobuf::connection::ConnectionConfigurationField* field = _configuration.add_fields();
	field->set_name(name);
	field->set_value(value.getValue());
	return true;
}

bool Configuration::updateAttribute(const std::string& name, const ghost::ConfigurationValue& value)
{
	auto iterator(_configuration.mutable_fields());
	for (auto it = iterator->begin(); it < iterator->end(); ++it)
	{
		if (it->name() == name)
		{
			it->set_value(value.getValue());
			return true;
		}
	}
	return false;
}

std::string Configuration::getAttribute(const std::string& name) const
{
	for (const ghost::protobuf::connection::ConnectionConfigurationField& str : _configuration.fields())
	{
		if (str.name() == name)
			return str.value();
	}
	return "";
}
