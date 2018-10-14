#ifndef GHOST_SUBSCRIBER_HPP
#define GHOST_SUBSCRIBER_HPP

#include <memory>

#include <ghost/connection/Connection.hpp>
#include <ghost/connection/Reader.hpp>

namespace ghost
{
	class Subscriber : public Connection
	{
	public:
		Subscriber(const ConnectionConfiguration& configuration);

		virtual ~Subscriber() = 0;

		template<typename MessageType>
		std::shared_ptr<Reader<MessageType>> getReader() // same as: using internal::Connection::getReader;
		{
			return internal::Connection::getReader<MessageType>();
		}
	};

	inline Subscriber::~Subscriber() {}
}

#endif // GHOST_SUBSCRIBER_HPP
