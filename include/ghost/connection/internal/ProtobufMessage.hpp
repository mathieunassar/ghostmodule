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

#ifndef GHOST_PROTOBUFMESSAGE_HPP
#define GHOST_PROTOBUFMESSAGE_HPP

#include <google/protobuf/message.h>

#include <ghost/connection/Message.hpp>
#include <memory>
#include <string>

namespace ghost
{
namespace internal
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
	virtual ~ProtobufMessage() = default;

	// convenience method to default initialize with protobuf message
	template <typename MessageType>
	static ProtobufMessage create();

	std::string getMessageFormatName() const override;
	std::string getMessageTypeName() const override;

	std::shared_ptr<google::protobuf::Message> getProtobufMessage() const;

	bool serialize(std::string& result) const override;
	bool deserialize(const std::string& payload) override;

private:
	std::shared_ptr<google::protobuf::Message> _payload;
};

// TEMPLATE DEFINITION //

template <typename MessageType>
ProtobufMessage ProtobufMessage::create()
{
	return ProtobufMessage(std::make_shared<MessageType>());
}

static const std::string GHOSTMESSAGE_FORMAT_NAME = "PROTOBUF";
} // namespace internal
} // namespace ghost

#endif // GHOST_PROTOBUFMESSAGE_HPP
