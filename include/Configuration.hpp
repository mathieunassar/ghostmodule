#ifndef GHOST_CONFIGURATION_HPP
#define GHOST_CONFIGURATION_HPP

#include <map>
#include "internal/ProtobufConfiguration.hpp"

namespace ghost
{
	/**
	 *	Contains a configuration consisting of a list of elements described by a name and a value.
	 */
	class Configuration
	{
	public:
		virtual ~Configuration() {}

		Configuration(const std::string& name = "");
		void copy(Configuration& to) const;

		const std::string& getConfigurationName() const;

		bool hasAttribute(const std::string& name) const;

		template<typename T>
		bool getAttribute(const std::string& name, T& value) const;
		template<typename T>
		bool getAttribute(const std::string& name, T& value, const T& defaultValue) const;
		std::map<std::string, std::string> getAttributes() const;

		bool removeAttribute(const std::string& name);

		template<typename T>
		bool addAttribute(const std::string& name, const T& value, bool overwrite = false);

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
