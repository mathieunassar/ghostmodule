#ifndef GHOST_MESSAGE_HPP
#define GHOST_MESSAGE_HPP

#include <string>

namespace ghost
{
	/**
	 * @brief Base class for messages that can be sent through a connection.
	 * 
	 * Connection objects expect either a message deriving this base type or
	 * a Google protobuf message.
	 */
	class Message
	{
	public:
		virtual ~Message() = 0;

		/**
		 * @brief accesses the format name.
		 * 
		 * The format name is used during the message serialization process.
		 * 
		 * @return the format name
		 */
		virtual std::string getMessageFormatName() const = 0;

		/**
		 * @brief accesses the message type name
		 * 
		 * @return the type name of the message
		 */
		virtual std::string getMessageTypeName() const = 0;

		/**
		 * @brief Serializes the message into a string.
		 * 
		 * @param result the serialized string
		 * @return true if the serialization succeeded.
		 * @return false otherwise
		 */
		virtual bool serialize(std::string& result) const = 0;

		/**
		 * @brief Deserializes the string into the message.
		 * 
		 * @param payload the string to deserialize.
		 * @return true if the message was successfully decoded into this object instance
		 * @return false if the deserialization failed.
		 */
		virtual bool deserialize(const std::string& payload) = 0;
	};

	inline Message::~Message() {}
}

#endif //GHOST_MESSAGE_HPP
