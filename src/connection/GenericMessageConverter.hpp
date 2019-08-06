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

#ifndef GHOST_GENERICMESSAGECONVERTER_HPP
#define GHOST_GENERICMESSAGECONVERTER_HPP

#include "../../../../protobuf/Ghost.pb.h"
#include "../../../../protobuf/Ghost.grpc.pb.h"
#include <ghost/connection/Message.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		 * Converter for GenericMessages and Messages. GenericMessage objects are protobuf messages
		 * which can contain any other protobuf message (namely represented by a protobuf::Any object)
		 * or a generic protobuf message containing a serialized version of a Message object in case the
		 * Message object is not based on protobuf. Message objects can either be instances of a
		 * ProtobufMessage, or implement a user defined protocol, from which the format name is also
		 * serialized into the GenericMessage.
		 * 
		 * The two exposed static methods convert a Message object into a GenericMessage, and parse a
		 * GenericMessage in the user provided Message instance.
		 * @author	Mathieu Nassar
		 * @date	12.06.2018
		 */
		class GenericMessageConverter
		{
		public:
			/**
			 * Converts the Message object into a GenericMessage protobuf message. If the conversion goes
			 * wrong, this method returns false.
			 * @author	Mathieu Nassar
			 * @date	12.06.2018
			 * @param [in,out]	message	a default constructed GenericMessage that will be filled by the
			 * 							method.
			 * @param 		  	from   	message to convert.
			 * @return	True if the conversion succeeded, false otherwise.
			 */
			static bool create(google::protobuf::Any& message, const ghost::Message& from);

			/**
			 * Parses the GenericMessage into the provided Message instance. If the conversion goes wrong,
			 * this method returns false.
			 * @author	Mathieu Nassar
			 * @date	12.06.2018
			 * @param 		  	message	message to convert.
			 * @param [in,out]	to	   	Message instance which will represent the converted message.
			 * @return	True if it succeeds, false if it fails.
			 */
			static bool parse(const google::protobuf::Any& message, ghost::Message& to);

			/**
			 * Gets the format name of the encapsulated message. If the message sent was originally a
			 * protobuf message, this call will return the value of internal::GHOSTMESSAGE_FORMAT_NAME.
			 * 
			 * If the message was serialized using a user format, then this call will return the name of
			 * this format.
			 * 
			 * If the message cannot be decoded, then this call returns "UNKNOWN".
			 * @author	Mathieu Nassar
			 * @date	12.06.2018
			 * @param	message	message to analyze.
			 * @return	The format name.
			 */
			static std::string getFormatName(const google::protobuf::Any& message);

			static std::pair<std::string, std::string> getFormatAndName(const google::protobuf::Any& message);

			/**
			 * Gets the name of the contained message without the host part of the Any URL.
			 * @author	Mathieu Nassar
			 * @date	14.06.2018
			 * @param	message	The message.
			 * @return	The true type name.
			 */
			static std::string getTrueTypeName(const google::protobuf::Any& message);
		};
	}
}

#endif // GHOST_GENERICMESSAGECONVERTER_HPP
