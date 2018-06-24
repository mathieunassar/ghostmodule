#include "../include/Configuration.hpp"

using namespace ghost;

Configuration::Configuration(const std::string& name)
	: _configuration(name)
{

}

const std::string& Configuration::getConfigurationName() const
{
	return _configuration.getConfigurationName();
}

bool Configuration::hasAttribute(const std::string& name) const
{
	return _configuration.hasAttribute(name);
}

bool Configuration::removeAttribute(const std::string& name)
{
	return _configuration.removeAttribute(name);
}
