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
#include <ghost/connection/internal/ProtobufConfiguration.hpp>

namespace ghost
{
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

		virtual ~Configuration() {}

		/**
		 * @brief Constructor of a Configuration, with the provided name.
		 * 
		 * @param name name of the configuration.
		 */
		Configuration(const std::string& name = "");

		/**
		 * @brief Copies the configuration into the provided configuration.
		 * 
		 * @param to destination configuration.
		 */
		void copy(Configuration& to) const;

		/**
		 * @return the name of the configuration. 
		 */
		const std::string& getConfigurationName() const;

		/**
		 * @brief Checks whether a parameter of the provided name exists in the configuration.
		 * 
		 * @param name name of the parameter to check
		 * @return true if a parameter with such a name exists
		 * @return false if no parameter with this name exists in the configuration.
		 */
		bool hasAttribute(const std::string& name) const;

		/**
		 * @brief Checks whether a parameter with the provided name exists and is empty.
		 * 
		 * @param name name of the parameter to check
		 * @return true if a parameter with this name exists and is empty
		 * @return false otherwise.
		 */
		bool isAttributeEmpty(const std::string& name) const;

		/**
		 * @brief gets the attribute with the given name if it exists.
		 * 
		 * @tparam T Expected type of the parameter.
		 * @param name Name of the parameter to get.
		 * @param value Value of the parameter (output value)
		 * @return true if the parameter was found and its value could be obtained in "value"
		 * @return false if the parameter was not found or if its value could not be casted to T
		 */
		template<typename T>
		bool getAttribute(const std::string& name, T& value) const;

		/**
		 * @brief get the attribute with the given name or a default value.
		 * 
		 * @tparam T Expected type of the parameter.
		 * @param name Name of the parameter to get.
		 * @param value Value of the parameter (output value)
		 * @param defaultValue default value to assign to "value" if the parameter is not found
		 * @return true if the parameter was found and its value could be obtained in "value"
		 * @return false if the parameter was not found or if its value could not be casted to T
		 */
		template<typename T>
		bool getAttribute(const std::string& name, T& value, const T& defaultValue) const;

		/**
		 * @return gets all the attributes and their values as strings. 
		 */
		std::map<std::string, std::string> getAttributes() const;

		/**
		 * @brief removes an attribute from the configuration.
		 * 
		 * @param name name of the attribute to remove
		 * @return true if an attribute was removed
		 * @return false if no attribute with such a name was found
		 */
		bool removeAttribute(const std::string& name);

		/**
		 * @brief adds an attribute with the given name and values.
		 * 
		 * @tparam T Type of the attribute's value
		 * @param name name of the attribute to add
		 * @param value value of the attribute to add
		 * @param overwrite if set to true, any attribute already existing with this name will be replaced
		 * @return true if the new attribute was added
		 * @return false if the new attribute was not added (i.e. an attribute already exists with this name
		 * and overwrite was set to false)
		 */
		template<typename T>
		bool addAttribute(const std::string& name, const T& value, bool overwrite = false);

		/**
		 * @brief updates the attribute with the given name to the given new value.
		 * 
		 * @tparam T Type of the attribute's value
		 * @param name name of the attribute to update
		 * @param value new value of this attribute
		 * @return true if an attribute was updated
		 * @return false if no attribute with the given name could be found.
		 */
		template<typename T>
		bool updateAttribute(const std::string& name, const T& value);

	private:
		internal::ProtobufConfiguration _configuration;
	};


	/* Template definition */

	template<typename T>
	bool Configuration::getAttribute(const std::string& name, T& value) const
	{
		return _configuration.getAttribute(name, value);
	}

	template<typename T>
	bool Configuration::getAttribute(const std::string& name, T& value, const T& defaultValue) const
	{
		return _configuration.getAttribute(name, value, defaultValue);
	}

	template<typename T>
	bool Configuration::addAttribute(const std::string& name, const T& value, bool overwrite)
	{
		return _configuration.addAttribute(name, value, overwrite);
	}

	template<typename T>
	bool Configuration::updateAttribute(const std::string& name, const T& value)
	{
		return _configuration.updateAttribute(name, value);
	}
}

#endif //GHOST_CONFIGURATION_HPP
