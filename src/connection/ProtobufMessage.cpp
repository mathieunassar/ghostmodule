#include "../include/ghost/connection/internal/ProtobufMessage.hpp"

using namespace ghost;

//const std::string internal::GHOSTMESSAGE_FORMAT_NAME = "PROTOBUF";

/**
 * Constructor of the message. The argument must not be nullptr or default initialized.
 * @author	Mathieu Nassar
 * @date	12.06.2018
 * @param	message	The protobuf message which is managed by this object.
 */
ProtobufMessage::ProtobufMessage(std::shared_ptr<google::protobuf::Message> message)
	: _payload(message)
{

}

/**
 * Gets the message format name.
 * @author	Mathieu Nassar
 * @date	12.06.2018
 * @return	the string "PROTOBUF", which value comes from the internal variable
 * 			"GHOSTMESSAGE_FORMAT_NAME".
 */
std::string ProtobufMessage::getMessageFormatName() const
{
	return internal::GHOSTMESSAGE_FORMAT_NAME;
}

/**
 * Gets the protobuf message type name.
 * @author	Mathieu Nassar
 * @date	12.06.2018
 * @return	The protobuf message type name.
 */
std::string ProtobufMessage::getMessageTypeName() const
{
	if (!_payload)
		return "";

	return _payload->GetTypeName();
}

/**
 * Gets the protobuf message.
 * @author	Mathieu Nassar
 * @date	12.06.2018
 * @return	The protobuf message.
 */
std::shared_ptr<google::protobuf::Message> ProtobufMessage::getProtobufMessage() const
{
	return _payload;
}

/**
 * Serialize this object to the given string and returns true on success.
 * Calls "SerializeToString" on the protobuf message.
 * @author	Mathieu Nassar
 * @date	12.06.2018
 * @param [in,out]	result	The serialized protobuf message.
 * @return	True if it succeeds, false if it fails.
 */
bool ProtobufMessage::serialize(std::string& result) const
{
	if (!_payload)
		return false;

	return _payload->SerializeToString(&result);
}

/**
 * Deserialize this object to the given string and returns true on success. Calls
 * "ParseFromString" on the existing protobuf message. If the protobuf message was not
 * previously set with the constructor, deserialization fails and the method returns false.
 * @author	Mathieu Nassar
 * @date	12.06.2018
 * @param	payload	The payload.
 * @return	True if it succeeds, false if it fails.
 */
bool ProtobufMessage::deserialize(const std::string& payload)
{
	if (!_payload)
		return false;
	
	return _payload->ParseFromString(payload);
}
