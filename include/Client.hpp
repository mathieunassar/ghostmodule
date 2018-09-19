#ifndef GHOST_CLIENT_HPP
#define GHOST_CLIENT_HPP

#include <memory>

#include "Connection.hpp"
#include "Message.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace ghost
{
	class Client : public Connection
	{
	public:
		Client(const ConnectionConfiguration& configuration);
		virtual ~Client() = 0 {}

		template<typename MessageType = ghost::Message>
		std::shared_ptr<Writer<MessageType>> getWriter() // same as: using internal::Connection::getWriter; 
		{
			return internal::Connection::getWriter<MessageType>();
		}

		template<typename MessageType = ghost::Message>
		std::shared_ptr<Reader<MessageType>> getReader() // same as: using internal::Connection::getReader; 
		{
			return internal::Connection::getReader<MessageType>();
		}
	};
}

#endif //GHOST_CLIENT_HPP
