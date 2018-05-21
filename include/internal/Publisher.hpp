#ifndef GHOST_INTERNAL_PUBLISHER_HPP
#define GHOST_INTERNAL_PUBLISHER_HPP

#include "Connection.hpp"

namespace Ghost
{
	namespace internal
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
		};
	}
}

#endif //GHOST_INTERNAL_PUBLISHER_HPP
