#ifndef GHOST_INTERNAL_CONSOLEDEVICE_HPP
#define GHOST_INTERNAL_CONSOLEDEVICE_HPP

#include <chrono>

namespace Ghost
{
	namespace internal
	{
		/**
		*	Abstract class describing the console window.
		*	Since its methods operate on a lower level, implementations
		*	of this class are operating system specific.
		*/
		class ConsoleDevice
		{
		public:
			/**
			*	Describes whether the user can see their input on the console
			*	or not.
			*/
			enum ConsoleMode
			{
				INPUT, /// The user can see their input in the console
				OUTPUT /// The user cannot see their input in the console
			};

			/**
			 *	Performs the necessary initialization actions.
			 *	@return true if the initialization is successful, false otherwise.
			 */
			virtual bool start() = 0;

			/**
			*	Switches the mode of the console window.
			*	Depending on the "mode" parameter, this method enables or diables
			*	the visualisation of text in the console window (@see ConsoleMode)
			*	@param mode		new mode to set
			*	@return true on success, false otherwise
			*/
			virtual bool setConsoleMode(ConsoleMode mode) = 0;

			/**
			 *	Waits that the user presses enter to switch to input mode.
			 *	@return true if the user pressed enter.
			 */
			virtual bool awaitInputMode() = 0;

			/**
			 *	Performs necessary operations on the local device to dispose it properly.
			 */
			virtual void stop() = 0;
		};
	}
}

#endif //GHOST_INTERNAL_CONSOLEDEVICE_HPP
