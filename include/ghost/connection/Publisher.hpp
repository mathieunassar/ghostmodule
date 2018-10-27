#ifndef GHOST_PUBLISHER_HPP
#define GHOST_PUBLISHER_HPP

#include <memory>

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/Writer.hpp>

namespace ghost
{
	/**
	 * @brief Specific type of Connection to send messages to Subscriber connections.
	 * 
	 * A Publisher can be created via a ConnectionManager, which will also
	 * manage its lifetime.
	 * 
	 * The configuration provided in the constructor is used to set up the connection
	 * parameters such as the target IP address, port number or threading or blocking
	 * parameters.
	 * 
	 * Particular implementations of Connection classes may define new connection
	 * parameters which can be passed through the ConnectionConfiguration.
	 */
	class Publisher : public Connection
	{
	public:
		/**
		 * @brief Construct a Publisher with the provided configuration.
		 * 
		 * @param configuration Publisher's configuration
		 */
		Publisher(const ConnectionConfiguration& configuration);

		virtual ~Publisher() = 0;

		/**
		 * @brief gets a Writer from the connection that writes messages of the provided
		 * type.
		 * 
		 * @tparam MessageType Message type written by the Writer objects.
		 * @return the requested writer, which will feed this connection.
		 */
		template<typename MessageType>
		std::shared_ptr<Writer<MessageType>> getWriter() // same as: using internal::Connection::getWriter;
		{
			return internal::Connection::getWriter<MessageType>();
		}
	};

	inline Publisher::~Publisher() {}
}

#endif //GHOST_PUBLISHER_HPP
