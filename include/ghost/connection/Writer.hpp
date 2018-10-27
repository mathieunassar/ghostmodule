#ifndef GHOST_WRITER_HPP
#define GHOST_WRITER_HPP

namespace ghost
{
	/**
	 * @brief Interface for writing through a connection.
	 * 
	 * Writer objects are obtained from connection objects.
	 * 
	 * The type of message being written by the Writer is provided
	 * by the template parameter "MessageType", which can be one of
	 * the following:
	 * - a Google Protobuf message type (including google.protobuf.Any)
	 * - A user-defined object type derived from the Message class.
	 * 
	 * @tparam MessageType the type of messages that will be written
	 * by the Writer.
	 */
	template<typename MessageType>
	class Writer
	{
	public:
		/**
		 * @brief Forwards the message to the connection which provided
		 * this Writer.
		 * 
		 * The default implementation pushes the message into a processing
		 * queue. Connection objects get the messages from the queue and
		 * write them sequentially.
		 * Messages are internally converted into google.protobuf.Any
		 * (eventually calling the serialize method of the Message class
		 * if applicable). If this conversion fails, this method returns
		 * false.
		 * 
		 * @param message the message to send
		 * @return true if the message was added to the processing queue
		 * @return false if the message was not added to the queue
		 */
		virtual bool write(const MessageType& message) = 0;
	};
}

#endif //GHOST_WRITER_HPP
