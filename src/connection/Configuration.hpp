/*
 * Copyright 2020 Mathieu Nassar
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

#ifndef GHOST_INTERNAL_CONFIGURATION_HPP
#define GHOST_INTERNAL_CONFIGURATION_HPP

#include <ghost/connection/Configuration.hpp>

#include "../../protobuf/ghost/connection/ConnectionConfiguration.pb.h"

namespace ghost
{
namespace internal
{
/**
 *	Implementation of the public API configuration class.
 */
class Configuration : public ghost::Configuration
{
public:
	Configuration(const std::string& name = "");

	void copy(ghost::Configuration& to) const override;
	const std::string& getConfigurationName() const override;
	bool hasAttribute(const std::string& name) const override;
	bool isAttributeEmpty(const std::string& name) const override;
	bool getAttribute(const std::string& name, ghost::ConfigurationValue& value) const override;
	bool getAttribute(const std::string& name, ghost::ConfigurationValue& value,
			  const ghost::ConfigurationValue& defaultValue) const override;
	std::map<std::string, std::string> getAttributes() const override;
	bool removeAttribute(const std::string& name) override;
	bool addAttribute(const std::string& name, const ghost::ConfigurationValue& value,
			  bool overwrite = false) override;
	bool updateAttribute(const std::string& name, const ghost::ConfigurationValue& value) override;

private:
	std::string getAttribute(const std::string& name) const;

	ghost::protobuf::connection::ConnectionConfiguration _configuration;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_CONFIGURATION_HPP
