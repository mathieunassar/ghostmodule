#ifndef GHOST_INTERNAL_GENERICWRITER_HPP
#define GHOST_INTERNAL_GENERICWRITER_HPP

#include <type_traits>

#include <ghost/connection/internal/WriterSink.hpp>
#include <ghost/connection/Writer.hpp>
#include <ghost/connection/Message.hpp>

#include <BlockingQueue.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		 *	Implementation of a writer of the template message type.
		 *	Its realization of the WriterSink interface allows connections to access the
		 *	messages to write, and the Writer interface represents the API implementation.
		 */
		template<typename MessageType>
		class GenericWriter : public WriterSink, public ghost::Writer<MessageType>
		{
		public:
			/// Constructor. The value of blocking determines whether calls to "write" will
			/// be blocking or not.
			GenericWriter(bool blocking);

			GenericWriter(const WriterSink& other, bool blocking);
			
			/// From WriterSink: gets the messages to write from the writer
			bool get(google::protobuf::Any& message, std::chrono::milliseconds timeout) override;

			void pop() override;

			/// converts and store the message into the message queue.
			/// If the writer is configured to be blocking, this method will only return once
			/// the message queue is empty (when the connection sent the message.
			bool write(const MessageType& message) override;

		private:
			template<class Q = MessageType>
			typename std::enable_if<std::is_same<google::protobuf::Any, Q>::value, bool>::type makeAny(google::protobuf::Any& any, const MessageType& message)
			{
				any = message;
				return true;
			}

			template<class Q = MessageType>
			typename std::enable_if<!std::is_base_of<ghost::Message, Q>::value
				&& !std::is_same<google::protobuf::Any, Q>::value, bool>::type makeAny(google::protobuf::Any& any, const MessageType& message)
			{
				return GenericMessageConverter::create(any, ghost::ProtobufMessage(std::make_shared<MessageType>(message)));
			}

			template<class Q = MessageType>
			typename std::enable_if<std::is_base_of<ghost::Message, Q>::value, bool>::type makeAny(google::protobuf::Any& any, const MessageType& message)
			{
				return GenericMessageConverter::create(any, message);
			}

			bool _blocking;
		};

		#include "GenericWriter.impl.hpp"
	}
}

#endif //GHOST_INTERNAL_GENERICWRITER_HPP
