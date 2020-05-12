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

#ifndef GHOST_COMMAND_PARAMETER_HPP
#define GHOST_COMMAND_PARAMETER_HPP

#include <string>

namespace ghost
{
/**
 *	Class representing a required or optional parameter for a ghost::Command.
 *	Contains information related to its name and descriptions.
 */
class CommandParameter
{
public:
	/**
	 *	Defines a parameter that can be used with a ghost::Command.
	 *	@param name	the name of this parameter for documentation purposes.
	 *	@param identifier the reference of this parmeter. It will be referenced by typing "--[identifier]"
	 *	@param shortIdentifier a shorter identifier for this parameter.
	 *		If not applicable, an empty string shall be provided.
	 *		If "shortIdentifier" is not empty, the ghost::CommandParameter may be referenced by
	 *		typing "-[shortIdentifier]".
	 *	@param description a description about how to use this parameter.
	 */
	CommandParameter(const std::string& name, const std::string& identifier, const std::string& shortIdentifier,
			 const std::string& descrition, bool expectsValue);

	/**
	 *	@return the name of this ghost::CommandParameter.
	 */
	const std::string& getName() const;
	/**
	 *	@return the identifier of this ghost::CommandParameter
	 */
	const std::string& getIdentifier() const;
	/**
	 *	@return the short identifier of this ghost::CommandParameter. An empty string means that this
	 *ghost::CommandParameter does not have a short identifier.
	 */
	const std::string& getShortIdentifier() const;
	/**
	 *	@return the description about how to use this ghost::CommandParameter.
	 */
	const std::string& getDescription() const;
	/**
	 *	@return true if this parameter shall expect a value after the parameter identifier.
	 */
	bool expectsValue() const;

private:
	std::string _name;
	std::string _identifier;
	std::string _shortIdentifier;
	std::string _description;
	bool _expectsValue;
};
} // namespace ghost

#endif // GHOST_COMMAND_HPP
