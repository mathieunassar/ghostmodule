#ifndef GHOST_MESSAGE_HPP
#define GHOST_MESSAGE_HPP

#include <string>

namespace ghost
{
	class Message
	{
	public:
		virtual ~Message() = 0 {}

		virtual std::string getMessageFormatName() const = 0;

		virtual std::string getMessageTypeName() const = 0;

		virtual bool serialize(std::string& result) const = 0;

		virtual bool deserialize(const std::string& payload) = 0;
	};
}

#endif //GHOST_MESSAGE_HPP
