#ifndef GHOST_CLIENT_HPP
#define GHOST_CLIENT_HPP

#include <memory>

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/Message.hpp>
#include <ghost/connection/Reader.hpp>
#include <ghost/connection/Writer.hpp>

namespace ghost
{
	class Client : public Connection
	{
	public:
		Client(const ConnectionConfiguration& configuration);
		virtual ~Client() = 0;

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

	inline Client::~Client() {}
}

#endif //GHOST_CLIENT_HPP
