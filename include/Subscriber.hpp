#ifndef GHOST_SUBSCRIBER_HPP
#define GHOST_SUBSCRIBER_HPP

#include <memory>

#include "internal/Subscriber.hpp"
#include "Connection.hpp"

namespace Ghost
{
	template<typename MessageType>
	class Subscriber : public Connection
	{			
	protected:
		/**
		 * Processes a message received.
		 * @author	Mathieu Nassar
		 * @date	21.05.2018
		 * @param	message	The message.
		 * @return	True if it succeeds, false if it fails.
		 */
		virtual bool onNewMessage(const MessageType& message) = 0;

	private:
		std::shared_ptr<internal::Subscriber<MessageType>> _internal;
	};
}

#endif // GHOST_SUBSCRIBER_HPP
