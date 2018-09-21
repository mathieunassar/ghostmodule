#ifndef GHOST_INTERNAL_GENERICREADER_HPP
#define GHOST_INTERNAL_GENERICREADER_HPP

#include <ghost/connection/internal/ReaderSink.hpp>
#include <ghost/connection/Reader.hpp>

#include <ghost/connection/internal/MessageHandler.hpp>
#include <ghost/connection/internal/GenericMessageConverter.hpp>
#include <ghost/connection/ProtobufMessage.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		 *	Implementation of a reader, that can read messages of the given templated type.
		 *	The reader gets the message from the ReaderSink interface, which is controlled
		 *	by a connection.
		 *	Its implementation of the Reader interface provides the API functionality.
		 */
		template<typename MessageType>
		class GenericReader : public ReaderSink, public ghost::Reader<MessageType>
		{
		public:
			/// Constructor. The value of blocking determines whether calls to "read" will
			/// be blocking or not.
			GenericReader(bool blocking);

			GenericReader(const ReaderSink& other, bool blocking);

			/// From ReaderSink: feeds the reader with messages to read
			bool put(const google::protobuf::Any& message) override;

			/// Reads the messages if possible. If the reader is set to be blocking, this call
			/// will only return once a message is fed into the reader.
			bool read(MessageType& message) override;
			/// reinterprets the last message read into the provided message format.
			bool lastRead(MessageType& message) override;

			/// setting a message handler will disable the read queue; further calls to read will
			/// return false, while new messages will automatically be processed by the message handler.
			std::shared_ptr<ghost::MessageHandler> addMessageHandler() override;

		private:
			template<class Q = MessageType>
			typename std::enable_if<!std::is_base_of<ghost::Message, Q>::value, bool>::type makeMessage(const google::protobuf::Any& any, MessageType& message)
			{
				auto proto = std::make_shared<MessageType>();
				ghost::ProtobufMessage container(proto);
				bool parseResult = GenericMessageConverter::parse(any, container);
				message = *proto;
				return parseResult;
			}

			template<class Q = MessageType>
			typename std::enable_if<std::is_base_of<ghost::Message, Q>::value, bool>::type makeMessage(const google::protobuf::Any& any, MessageType& message)
			{
				return GenericMessageConverter::parse(any, message);
			}

			bool _blocking;

			std::shared_ptr<MessageHandler> _messageHandler;
			google::protobuf::Any _last;
		};

		#include "GenericReader.impl.hpp"
	}
}

#endif //GHOST_INTERNAL_GENERICREADER_HPP
