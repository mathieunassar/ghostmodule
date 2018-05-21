#ifndef GHOST_CONNECTION_HPP
#define GHOST_CONNECTION_HPP

#include "internal/Connection.hpp"

namespace Ghost
{
	/**
	 * A connection.
	 * @author	Mathieu Nassar
	 * @date	21.05.2018
	 */
	class Connection
	{
	public:
		virtual ~Connection() = 0; // the class is pure virtual

		/**
		 * If the start sequence is successful, then the connection is running.
		 * @author	Mathieu Nassar
		 * @date	21.05.2018
		 * @return	true if the start sequence was successful, false otherwise.
		 */
		bool start();

		/**
		 * If the stop sequence is successful, then the connection is stopped.
		 * @author	Mathieu Nassar
		 * @date	21.05.2018
		 * @return	true if the stop sequence was successful, false otherwise.
		 */
		bool stop();

		/**
		 * Query if this object is running.
		 * @author	Mathieu Nassar
		 * @date	21.05.2018
		 * @return	true if the connection is currently running, false otherwise.
		 */
		bool isRunning() const;

	protected:
		Connection(std::shared_ptr<internal::Connection> internal);
	
	private:
		std::shared_ptr<internal::Connection> _internal;
	};

	inline Connection::~Connection() {}
}

#endif //GHOST_CONNECTION_HPP
