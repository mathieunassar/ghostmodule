#ifndef GHOST_PUBLISHER_HPP
#define GHOST_PUBLISHER_HPP

#include <memory>

#include "Connection.hpp"
#include "Writer.hpp"

namespace ghost
{
	class Publisher : public Connection
	{
	public:
		Publisher(const ConnectionConfiguration& configuration);

		virtual ~Publisher() = 0 {}
		
		template<typename MessageType>
		std::shared_ptr<Writer<MessageType>> getWriter() // same as: using internal::Connection::getWriter;
		{
			return internal::Connection::getWriter<MessageType>();
		}
	};
}

#endif //GHOST_PUBLISHER_HPP
