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
	 * Base class for message handlers.
	 * @author	Mathieu Nassar
	 * @date	15.06.2018
	 */
	class MessageHandler
	{
	public:
		virtual ~MessageHandler() = 0;

		template<typename MessageType>
		void addHandler(std::function<void(const MessageType& message)> handler)
		{
			_internal->registerHandler(handler);
		}

	protected:
		internal::MessageHandler* _internal;
	};

	inline MessageHandler::~MessageHandler() {}
}

#endif //GHOST_MESSAGEHANDLER_HPP
