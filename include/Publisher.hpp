#ifndef GHOST_PUBLISHER_HPP
#define GHOST_PUBLISHER_HPP

#include <memory>

#include "internal/Publisher.hpp"
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
		void publish(const MessageType& message);

	private:
		std::shared_ptr<internal::Publisher<MessageType>> _internal;
	};
}

#endif //GHOST_PUBLISHER_HPP
