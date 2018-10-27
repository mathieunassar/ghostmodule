#ifndef GHOST_INTERNAL_MESSAGEHANDLERCALLBACK_HPP
#define GHOST_INTERNAL_MESSAGEHANDLERCALLBACK_HPP

#include <functional>
#include <google/protobuf/any.pb.h>

#include <ghost/connection/Message.hpp>
#include <ghost/connection/internal/ProtobufMessage.hpp>
#include <ghost/connection/internal/GenericMessageConverter.hpp>

namespace ghost
{
	namespace internal
	{
		/**
		 *	Base class for the handlers
		 */
		class BaseMessageHandlerCallback
		{
		public:
			virtual void handle(const google::protobuf::Any& message) = 0;
		};

		template<typename MessageType, bool = std::is_base_of<ghost::Message, MessageType>::value>
		class MessageHandlerCallback;

		/// Implementation for protobuf messages
		template<typename MessageType>
		class MessageHandlerCallback<MessageType, false> : public BaseMessageHandlerCallback
		{
		public:
			MessageHandlerCallback(std::function<void(const MessageType& message)> callback)
				: _callback(callback) {}

			void handle(const google::protobuf::Any& message) override
			{
				auto proto = std::make_shared<MessageType>();
				ghost::ProtobufMessage msg(proto);
				bool parseSuccess = GenericMessageConverter::parse(message, msg);
				if (parseSuccess)
					_callback(*proto);
			}

		private:
			std::function<void(const MessageType& message)> _callback;
		};

		/// Implementation for ghost::Message
		template<typename MessageType>
		class MessageHandlerCallback<MessageType, true> : public BaseMessageHandlerCallback
		{
		public:
			MessageHandlerCallback(std::function<void(const MessageType& message)> callback)
				: _callback(callback) {}

			void handle(const google::protobuf::Any& message) override
			{
				MessageType msg;
				bool parseSuccess = GenericMessageConverter::parse(message, msg);
				if (parseSuccess)
					_callback(msg);
			}

		private:
			std::function<void(const MessageType& message)> _callback;
		};
	}
}

#endif //GHOST_INTERNAL_MESSAGEHANDLERCALLBACK_HPP
