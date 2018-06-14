#ifndef GHOST_PROTOBUFMESSAGE_HPP
#define GHOST_PROTOBUFMESSAGE_HPP

#include "Message.hpp"
#include <google/protobuf/message.h>

namespace ghost
{
	/**
	 * Message based on the Google Protobuf library. Wraps a protobuf message and can be sent
	 * through the connection library.
	 * @author	Mathieu Nassar
	 * @date	12.06.2018
	 */
	class ProtobufMessage : public Message
	{
	public:
		ProtobufMessage(std::shared_ptr<google::protobuf::Message> message);

		// convenience method to default initialize with protobuf message
		template<typename MessageType>
		static ProtobufMessage create();

		std::string getMessageFormatName() const override;
		std::string getMessageTypeName() const override;

		std::shared_ptr<google::protobuf::Message> getProtobufMessage() const;

		bool serialize(std::string& result) const override;
		bool deserialize(const std::string& payload) override;

	private:
		std::shared_ptr<google::protobuf::Message> _payload;
	};

	//////////////////////////////////////////////////////////////////////////////////////////
	/******************************** Template definition ***********************************/
	//////////////////////////////////////////////////////////////////////////////////////////

	template<typename MessageType>
	ProtobufMessage ProtobufMessage::create()
	{
		return ProtobufMessage(std::make_shared<MessageType>());
	}

	namespace internal
	{
		static const std::string GHOSTMESSAGE_FORMAT_NAME = "PROTOBUF";
	}
}

#endif // GHOST_PROTOBUFMESSAGE_HPP
