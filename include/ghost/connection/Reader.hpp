#ifndef GHOST_READER_HPP
#define GHOST_READER_HPP

#include <ghost/connection/Message.hpp>
#include <ghost/connection/MessageHandler.hpp>

namespace ghost
{
	/**
	 * @brief Interface for reading through a connection.
	 * 
	 * Reader objects are obtained from connection objects.
	 * 
	 * The type of message being read by the Reader is provided
	 * by the template parameter "MessageType", which can be one of
	 * the following:
	 * - a Google Protobuf message type (including google.protobuf.Any)
	 * - A user-defined object type derived from the Message class.
	 * 
	 * @tparam MessageType the type of messages that will be read
	 * by the Reader.
	 */
	template<typename MessageType>
	class Reader
	{
	public:
		/**
		 * @brief Reads a message from the reader.
		 * 
		 * If a message handler was added to this class, then calling read will
		 * always return false.
		 * Read will also return false if the connection was set to not block during
		 * I/O calls.
		 * Finally, the method will return false if the incoming message does not match
		 * the expected type MessageType.
		 * In all the other cases, this method returns true.
		 * 
		 * @param message the message to read (input)
		 * @return true if a message of the given type was actually read
		 * @return false if no message was read during this call.
		 */
		virtual bool read(MessageType& message) = 0;

		/**
		 * @brief Gets the last read message.
		 * 
		 * @param message the message to read (input)
		 * @return true if a message of the given type was actually read
		 * @return false if no message could be returned
		 */
		virtual bool lastRead(MessageType& message) = 0;

		/**
		 * @brief Adds a message handler to the reader and returns it.
		 * 
		 * See the MessageHandler documentation for more information. If a message
		 * handler is set to this connection, subsequent calls to "read" will return
		 * false.
		 * 
		 * After the message handler was added, handler functions can be added to it.
		 * 
		 * @return the message handler that was added to the reader. 
		 */
		virtual std::shared_ptr<ghost::MessageHandler> addMessageHandler() = 0;
	};
}

#endif //GHOST_WRITER_HPP
