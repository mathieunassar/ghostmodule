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

#include <ghost/connection/internal/GenericMessageConverter.hpp>
#include <ghost/connection/internal/ProtobufMessage.hpp>

#include "../../protobuf/ghost/connection/GenericMessage.pb.h"

using namespace ghost::internal;

bool GenericMessageConverter::create(google::protobuf::Any& message, const ghost::Message& from)
{
	std::shared_ptr<google::protobuf::Message> payload;

	std::string formatName = from.getMessageFormatName();
	if (formatName == internal::GHOSTMESSAGE_FORMAT_NAME) // this is already a protobuf message
	{
		const ghost::internal::ProtobufMessage& fromProtobuf =
		    static_cast<const ghost::internal::ProtobufMessage&>(from);
		payload = fromProtobuf.getProtobufMessage();

		if (!payload)
		{
			return false; // there is no message in this source, return false as documented
		}
	}
	else // this is a user format
	{
		std::string fromSerialized;
		bool serializationSuccess = from.serialize(fromSerialized);
		if (!serializationSuccess)
		{
			return false; // serialization failed, return false as documented
		}

		auto defaultPayload = std::make_shared<protobuf::connection::GenericMessage>();
		defaultPayload->set_format(formatName);
		defaultPayload->set_name(from.getMessageTypeName());
		defaultPayload->set_serial(fromSerialized);
		payload = defaultPayload;
	}

	// fill the message
	message.PackFrom(*payload);

	return true;
}

bool GenericMessageConverter::parse(const google::protobuf::Any& message, ghost::Message& to)
{
	std::string targetFormatName = to.getMessageFormatName();
	std::string targetTypeName = to.getMessageTypeName();

	if (getTrueTypeName(message) ==
	    protobuf::connection::GenericMessage::descriptor()->full_name()) // payload is of a user defined format
	{
		protobuf::connection::GenericMessage defaultPayload;
		bool unpackSuccess = message.UnpackTo(&defaultPayload);
		if (!unpackSuccess)
		{
			return false; // failed to convert Any to default payload, return false as documented
		}

		if (defaultPayload.format() != targetFormatName)
		{
			return false; // source format is different than the format expected by the user, return false
				      // as documented
		}

		if (defaultPayload.name() != targetTypeName)
		{
			return false;
		}

		// deserialize
		return to.deserialize(defaultPayload.serial());
	}

	// else, payload is a protobuf message
	if (targetFormatName != internal::GHOSTMESSAGE_FORMAT_NAME)
	{
		return false; // payload is a protobuf message, but user expected something else, return false as
			      // documented
	}

	ghost::internal::ProtobufMessage& toProtobuf = static_cast<ghost::internal::ProtobufMessage&>(to);

	if (!toProtobuf.getProtobufMessage())
	{
		return false; // to does not contain a protobuf message to be loaded to
	}

	// deserialize
	return message.UnpackTo(toProtobuf.getProtobufMessage().get());
}

std::string GenericMessageConverter::getFormatName(const google::protobuf::Any& message)
{
	// if the any message is not a default payload, then it's already a protobuf message
	if (getTrueTypeName(message) != protobuf::connection::GenericMessage::descriptor()->full_name())
	{
		return internal::GHOSTMESSAGE_FORMAT_NAME;
	}

	protobuf::connection::GenericMessage defaultPayload;
	bool unpackSuccess = message.UnpackTo(&defaultPayload);
	if (!unpackSuccess)
	{
		return "UNKNOWN"; // cannot convert it back to a default payload although it is not a native protobuf
				  // message... who sent the message!?
	}

	return defaultPayload.format(); // return the format stored in the default payload message
}

std::pair<std::string, std::string> GenericMessageConverter::getFormatAndName(const google::protobuf::Any& message)
{
	std::string format, name;

	// if the any message is not a default payload, then it's already a protobuf message
	if (getTrueTypeName(message) != protobuf::connection::GenericMessage::descriptor()->full_name())
	{
		format = internal::GHOSTMESSAGE_FORMAT_NAME;
		name = getTrueTypeName(message);
	}
	else
	{
		protobuf::connection::GenericMessage defaultPayload;
		bool unpackSuccess = message.UnpackTo(&defaultPayload);
		if (!unpackSuccess)
		{
			format = "UNKNOWN"; // cannot convert it back to a default payload although it is not a native
					    // protobuf message... who sent the message!?
			name = "UNKNOWN";
		}
		else
		{
			format = defaultPayload.format();
			name = defaultPayload.name();
		}
	}

	return std::make_pair(format, name);
}

std::string GenericMessageConverter::getTrueTypeName(const google::protobuf::Any& message)
{
	const std::string& url = message.type_url();
	size_t last = url.find_last_of('/');
	if (last == std::string::npos)
	{
		return url;
	}
	return url.substr(last + 1);
}
