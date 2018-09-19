#include "../include/internal/ConnectionFactoryRule.hpp"

using namespace ghost::internal;

ConnectionFactoryRule::ConnectionFactoryRule(const ghost::ConnectionConfiguration& minimumConfiguration)
	: _minimumConfiguration(minimumConfiguration)
{

}

bool ConnectionFactoryRule::matches(const ghost::ConnectionConfiguration& candidate) const
{
	auto attributes = _minimumConfiguration.getAttributes();
	for (auto& attribute : attributes)
	{
		std::string candidateValue;
		bool hasAttribute = candidate.getAttribute(attribute.first, candidateValue);

		if (!hasAttribute)
			return false; // the candidate does not have this attribute

		if (!attribute.second.empty() // an empty value in the minimum configuration means that any param is allowed, otherwise it must match
			&& attribute.second.compare(candidateValue) != 0)
			return false; // the candidate has a different value than the non empty min configuration
	}

	return true;
}