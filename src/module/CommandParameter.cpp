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

#include <ghost/module/CommandParameter.hpp>

using namespace ghost;

CommandParameter::CommandParameter(const std::string& name, const std::string& identifier,
				   const std::string& shortIdentifier, const std::string& descrition, bool expectsValue)
    : _name(name)
    , _identifier(identifier)
    , _shortIdentifier(shortIdentifier)
    , _description(descrition)
    , _expectsValue(expectsValue)
{
}

const std::string& CommandParameter::getName() const
{
	return _name;
}

const std::string& CommandParameter::getIdentifier() const
{
	return _identifier;
}

const std::string& CommandParameter::getShortIdentifier() const
{
	return _shortIdentifier;
}

const std::string& CommandParameter::getDescription() const
{
	return _description;
}

bool CommandParameter::expectsValue() const
{
	return _expectsValue;
}
