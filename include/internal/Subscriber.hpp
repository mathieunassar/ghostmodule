#ifndef GHOST_INTERNAL_SUBSCRIBER_HPP
#define GHOST_INTERNAL_SUBSCRIBER_HPP

#include "Connection.hpp"

namespace ghost
{
	namespace internal
	{
		template<typename MessageType>
		class Subscriber : public Connection
		{
		};
	}
}

#endif // GHOST_INTERNAL_SUBSCRIBER_HPP
