#ifndef GHOST_WRITER_HPP
#define GHOST_WRITER_HPP

namespace ghost
{
	/**
	 *	User interface to writing through connections.
	 */
	template<typename MessageType>
	class Writer
	{
	public:
		virtual bool write(const MessageType& message) = 0;
	};
}

#endif //GHOST_WRITER_HPP
