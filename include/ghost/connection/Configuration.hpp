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

#ifndef GHOST_CONFIGURATION_HPP
#define GHOST_CONFIGURATION_HPP

#include <map>
#include <memory>
#include <sstream>
#include <string>

namespace ghost
{
/**
 *	Contains a value used in the configuration.
 */
class ConfigurationValue
{
public:
	ConfigurationValue(const std::string& stringValue = "");

	/**
	 *	Reads the value and parses into the provided parameter.
	 *	@Tparam the type of this configuration value to read.
	 *	@param value	if the operation is successful, "value" will contain the read value.
	 *	@return true if the value could be parsed in the provided parameter.
	 */
	template <typename T>
	bool read(T& value) const;

	/**
	 *	Write a new value into this configuration value.
	 *	@Tparam the type of the new value to be written to the configuration. It must be possible
	 *	for std::ostringstream to parse this value to a string.
	 *	@param value	the new value to write in this configuration.
	 */
	template <typename T>
	void write(const T& value);

	/**
	 *	@return the value contained in this object as a string.
	 */
	const std::string& getValue() const;

private:
	std::string _value;
};

/**
 *	Contains a configuration consisting of a list of elements described by a name and a value.
 */
class Configuration
{
public:
	/**
	 * @brief value of an empty attribute.
	 */
	static const std::string EMPTY;

	virtual ~Configuration() = default;

	/**
	 * @brief Constructor of a Configuration, with the provided name.
	 *
	 * @param name name of the configuration.
	 */
	static std::shared_ptr<Configuration> create(const std::string& name = "");

	/**
	 * @brief Copies the configuration into the provided configuration.
	 *
	 * @param to destination configuration.
	 */
	virtual void copy(Configuration& to) const = 0;

	/**
	 * @return the name of the configuration.
	 */
	virtual const std::string& getConfigurationName() const = 0;

	/**
	 * @brief Checks whether a parameter of the provided name exists in the configuration.
	 *
	 * @param name name of the parameter to check
	 * @return true if a parameter with such a name exists
	 * @return false if no parameter with this name exists in the configuration.
	 */
	virtual bool hasAttribute(const std::string& name) const = 0;

	/**
	 * @brief Checks whether a parameter with the provided name exists and is empty.
	 *
	 * @param name name of the parameter to check
	 * @return true if a parameter with this name exists and is empty
	 * @return false otherwise.
	 */
	virtual bool isAttributeEmpty(const std::string& name) const = 0;

	/**
	 * @brief gets the attribute with the given name if it exists.
	 *
	 * @param name Name of the parameter to get.
	 * @param value Value of the parameter (output value)
	 * @return true if the parameter was found and its value could be obtained in "value"
	 * @return false if the parameter was not found
	 */
	virtual bool getAttribute(const std::string& name, ConfigurationValue& value) const = 0;

	/**
	 * @brief get the attribute with the given name or a default value.
	 *
	 * @param name Name of the parameter to get.
	 * @param value Value of the parameter (output value)
	 * @param defaultValue default value to assign to "value" if the parameter is not found
	 * @return true if the parameter was found and its value could be obtained in "value"
	 * @return false if the parameter was not found
	 */
	virtual bool getAttribute(const std::string& name, ConfigurationValue& value,
				  const ConfigurationValue& defaultValue) const = 0;

	/**
	 * @return gets all the attributes and their values as strings.
	 */
	virtual std::map<std::string, std::string> getAttributes() const = 0;

	/**
	 * @brief removes an attribute from the configuration.
	 *
	 * @param name name of the attribute to remove
	 * @return true if an attribute was removed
	 * @return false if no attribute with such a name was found
	 */
	virtual bool removeAttribute(const std::string& name) = 0;

	/**
	 * @brief adds an attribute with the given name and values.
	 *
	 * @param name name of the attribute to add
	 * @param value value of the attribute to add
	 * @param overwrite if set to true, any attribute already existing with this name will be replaced
	 * @return true if the new attribute was added
	 * @return false if the new attribute was not added (i.e. an attribute already exists with this name
	 * and overwrite was set to false)
	 */
	virtual bool addAttribute(const std::string& name, const ConfigurationValue& value, bool overwrite = false) = 0;

	/**
	 * @brief updates the attribute with the given name to the given new value.
	 *
	 * @param name name of the attribute to update
	 * @param value new value of this attribute
	 * @return true if an attribute was updated
	 * @return false if no attribute with the given name could be found.
	 */
	virtual bool updateAttribute(const std::string& name, const ConfigurationValue& value) = 0;
};

//// TEMPLATE DEFINITION ////

template <typename T>
bool ConfigurationValue::read(T& value) const
{
	T tmp{};
	std::istringstream iss(_value);
	iss >> tmp;

	if (!iss.fail())
	{
		value = tmp;
		return true;
	}

	return false;
}

template <typename T>
void ConfigurationValue::write(const T& value)
{
	std::ostringstream oss;
	oss << value;
	_value = oss.str();
}
} // namespace ghost

#endif // GHOST_CONFIGURATION_HPP
