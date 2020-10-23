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

#include <ghost/connection/ConnectionFactoryRule.hpp>

using namespace ghost;

ConnectionFactoryRule::ConnectionFactoryRule(const ghost::ConnectionConfiguration& minimumConfiguration)
    : _minimumConfiguration(minimumConfiguration)
{
}

bool ConnectionFactoryRule::matches(const ghost::ConnectionConfiguration& candidate) const
{
	auto attributes = _minimumConfiguration.getConfiguration()->getAttributes();
	for (auto& attribute : attributes)
	{
		std::string candidateValueString;
		ghost::ConfigurationValue candidateValue;
		bool hasAttribute = candidate.getConfiguration()->hasAttribute(attribute.first);
		bool isAttributeEmpty = _minimumConfiguration.getConfiguration()->isAttributeEmpty(attribute.first);
		candidate.getConfiguration()->getAttribute(attribute.first, candidateValue);
		candidateValue.read<std::string>(candidateValueString);

		if (!hasAttribute) return false; // the candidate does not have this attribute

		if (!isAttributeEmpty // an empty value in the minimum configuration means that any param is allowed,
				      // otherwise it must match
		    && attribute.second.compare(candidateValueString) != 0)
			return false; // the candidate has a different value than the non empty min configuration
	}

	return true;
}