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

#ifndef GHOST_COMMANDLINE_HPP
#define GHOST_COMMANDLINE_HPP

#include <exception>
#include <ghost/module/CommandParameter.hpp>
#include <map>
#include <sstream>
#include <string>

namespace ghost
{
/**
 *	This class represents a set of parameters used as an input for a command.
 *
 *	The name of the command can be retrieved with "getCommandName".
 *	Existence of parameters should be checked with "hasParameter" before they are queried.
 *
 *	During the parsing process, unnamed parameter still exist in the map under the name
 *	"__x", where x is the parameter's position in the command line (starting with zero).
 */
class CommandLine
{
public:
	CommandLine(const std::string& commandName = "", const std::map<std::string, std::string>& parameters = {});

	/**
	 *	@return the name of this command.
	 */
	const std::string& getCommandName() const;
	/**
	 *	Checks the existence of a parameter under the given name.
	 *	@param parameterName	Name of the parameter.
	 *	@return true if a parameter exists with this name, false otherwise.
	 */
	bool hasParameter(const std::string& parameterName) const;
	/**
	 *	Checks the existence of a parameter.
	 *	@param parameter	Parameter to check.
	 *	@return true if the parameter exists in the command line, false otherwise.
	 */
	bool hasParameter(const ghost::CommandParameter& parameter) const;
	/**
	 *	Retrieves a parameter with the given name.
	 *	As parameters are stored as strings, the template parameter is used to cast the string into a desired
	 *	type. For this purpose, a std::istringstream is used. Compatible types are types compatible with this
	 *	conversion method.
	 *	@param parameterName	Name of the parameter.
	 *	@return the target parameter, converted into the target type.
	 */
	template <typename T>
	T getParameter(const std::string& parameterName) const;
	/**
	 *	Retrieves a parameter.
	 *	As parameters are stored as strings, the template parameter is used to cast the string into a desired
	 *	type. For this purpose, a std::istringstream is used. Compatible types are types compatible with this
	 *	conversion method.
	 *	@param parameter	References the parameter to retrieve.
	 *	@return the target parameter, converted into the target type.
	 */
	template <typename T>
	T getParameter(const ghost::CommandParameter& parameter) const;
	/**
	 *	@returns the map of parameters for this command line as strings.
	 */
	const std::map<std::string, std::string>& getParametersMap() const;

private:
	std::string _commandName;
	std::map<std::string, std::string> _parameters;
};

// Template definition //

template <typename T>
T CommandLine::getParameter(const std::string& parameterName) const
{
	// sanity check, the user should first check if this object has the parameter with "hasParameter"
	if (_parameters.count(parameterName) == 0)
		throw std::invalid_argument("No parameter matches the provided name");

	std::istringstream iss(_parameters.at(parameterName));
	T val;
	iss >> val; // try to cast it
	return val;
}

template <typename T>
T CommandLine::getParameter(const ghost::CommandParameter& parameter) const
{
	if (hasParameter(parameter.getIdentifier()))
		return getParameter<T>(parameter.getIdentifier());
	else if (hasParameter(parameter.getShortIdentifier()))
		return getParameter<T>(parameter.getShortIdentifier());

	throw std::invalid_argument("No parameter matches the provided name");
}
} // namespace ghost

#endif // GHOST_COMMANDLINE_HPP
