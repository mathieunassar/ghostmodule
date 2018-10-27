#ifndef GHOST_CLIENT_HPP
#define GHOST_CLIENT_HPP

#include <memory>

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/Message.hpp>
#include <ghost/connection/Reader.hpp>
#include <ghost/connection/Writer.hpp>

namespace ghost
{
	/**
	 * @brief Client that connects to a Server.
	 * 
	 * A Client can be created via a ConnectionManager, which will also
	 * manage its lifetime.
	 * 
	 * The configuration provided in the constructor is used to set up the connection
	 * parameters such as the target IP address, port number or threading or blocking
	 * parameters.
	 * 
	 * Particular implementations of Connection classes may define new connection
	 * parameters which can be passed through the ConnectionConfiguration.
	 */
	class Client : public Connection
	{
	public:
		Client(const ConnectionConfiguration& configuration);
		virtual ~Client() = 0;

		/**
		 * @brief gets a Writer from the connection that writes messages of the provided
		 * type.
		 * 
		 * @tparam MessageType Message type written by the Writer objects.
		 * @return the requested writer, which will feed this connection.
		 */
		template<typename MessageType = ghost::Message>
		std::shared_ptr<Writer<MessageType>> getWriter() // same as: using internal::Connection::getWriter;
		{
			return internal::Connection::getWriter<MessageType>();
		}

		/**
		 * @brief gets a Reader from the connection that reads messages of the provided
		 * type.
		 * 
		 * @tparam MessageType Message type read by the Reader objects.
		 * @return the requested reader, which will be fed by this connection.
		 */
		template<typename MessageType = ghost::Message>
		std::shared_ptr<Reader<MessageType>> getReader() // same as: using internal::Connection::getReader;
		{
			return internal::Connection::getReader<MessageType>();
		}
	};

	inline Client::~Client() {}
}

#endif //GHOST_CLIENT_HPP
