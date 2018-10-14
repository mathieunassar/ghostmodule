#ifndef GHOST_PUBLISHER_HPP
#define GHOST_PUBLISHER_HPP

#include <memory>

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/Writer.hpp>

namespace ghost
{
	class Publisher : public Connection
	{
	public:
		Publisher(const ConnectionConfiguration& configuration);

		virtual ~Publisher() = 0;

		template<typename MessageType>
		std::shared_ptr<Writer<MessageType>> getWriter() // same as: using internal::Connection::getWriter;
		{
			return internal::Connection::getWriter<MessageType>();
		}
	};

	inline Publisher::~Publisher() {}
}

#endif //GHOST_PUBLISHER_HPP
