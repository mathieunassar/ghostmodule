#ifndef GHOST_CONNECTION_HPP
#define GHOST_CONNECTION_HPP

namespace ghost
{
	/**
	 * A connection.
	 * @author	Mathieu Nassar
	 * @date	21.05.2018
	 */
	class Connection
	{
	public:
		virtual ~Connection() {}

		/**
		* If the start sequence is successful, then the connection is running.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		* @return	true if the start sequence was successful, false otherwise.
		*/
		virtual bool start() = 0;

		/**
		* If the stop sequence is successful, then the connection is stopped.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		* @return	true if the stop sequence was successful, false otherwise.
		*/
		virtual bool stop() = 0;

		/**
		* Query if this object is running.
		* @author	Mathieu Nassar
		* @date	21.05.2018
		* @return	true if the connection is currently running, false otherwise.
		*/
		virtual bool isRunning() const = 0;
	};
}

#endif //GHOST_CONNECTION_HPP
