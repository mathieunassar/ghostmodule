#ifndef GHOST_PUBLISHER_HPP
#define GHOST_PUBLISHER_HPP

#include <memory>

#include "Connection.hpp"

namespace ghost
{
	template<typename MessageType>
	class Publisher : public Connection
	{
	public:
		/**
		 * Publishes the given message.
		 * @author	Mathieu Nassar
		 * @date	21.05.2018
		 * @param	message	The message.
		 */
		virtual void publish(const MessageType& message) = 0;
	};
}

#endif //GHOST_PUBLISHER_HPP
