#ifndef GHOST_INTERNAL_CONNECTION_HPP
#define GHOST_INTERNAL_CONNECTION_HPP

namespace Ghost
{
	/* goal: create a connection like: std::shared_ptr<MySubscriber> subscriber = _connectionManager->create<MySubscriber>(config);
	the connection manager would instantiate the class, create the corresponding core (using the config & the connection type).
	Implies that the internal::connection should be able to set a core to the newly created instance.
	Process: Instanciation of template parameter -> core creation using parameters -> core assignment to the instance -> return
	Problem: user cannot use a complex constructor
	*/

	/* Goal update: user can derive any connection type, and create them without connection manager.
	USer needs however to provide the connection core manually -> needs abstraction layers (NetworkServer for example, which takes directly connection parameters
	and creates the core correspondingly */

	namespace internal
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
		};
	}
}

#endif //GHOST_CONNECTION_HPP
