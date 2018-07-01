#ifndef GHOST_SUBSCRIBER_HPP
#define GHOST_SUBSCRIBER_HPP

#include <memory>

#include "Connection.hpp"
#include "MessageHandler.hpp"

namespace ghost
{
	template<typename MessageType>
	class Subscriber : public Connection
	{			
	public:
		virtual void setMessageHandler(std::function<void(const MessageType& message)> handler) = 0;
	};
}

#endif // GHOST_SUBSCRIBER_HPP
