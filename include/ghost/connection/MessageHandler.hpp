#ifndef GHOST_MESSAGEHANDLER_HPP
#define GHOST_MESSAGEHANDLER_HPP

#include <memory>
#include <functional>

namespace ghost
{
	namespace internal
	{
		class MessageHandler;
	}

	/**
	 * @brief Collection of handlers for incoming messages.
	 * 
	 * An implementation of this class can be obtained by calling
	 * "addMessageHandler" on Writer objects. Calling this method will forward
	 * all new received messages to this message handler.
	 * 
	 * The method "addHandler" can be used to set up handlers for specific types
	 * of messages. The type of message is passed as a template parameter to this
	 * call.
	 */
	class MessageHandler
	{
	public:
		virtual ~MessageHandler() = 0;

		/**
		 * @brief Adds a handler that processes messages of the templated type.
		 * 
		 * If a handler already exists for the given message type, it will be replaced.
		 * 
		 * @tparam MessageType the type of messages handled by this handler
		 * @param handler the handler that can handle the messages of this type
		 */
		template<typename MessageType>
		void addHandler(std::function<void(const MessageType& message)> handler);

	protected:
		internal::MessageHandler* _internal;
	};

	inline MessageHandler::~MessageHandler() {}
}

#endif //GHOST_MESSAGEHANDLER_HPP
