#ifndef GHOST_OUTPUTCONTROLLER_HPP
#define GHOST_OUTPUTCONTROLLER_HPP

#include <string>

namespace Ghost
{
	/**
	 *	Interface defining methods to manage user input.
	 */
	class OutputController
	{
	public:
		/// starts the output control
		virtual void start() = 0;
		/// stops the output control
		virtual void stop() = 0;
		/// resumes the output
		virtual void enable() = 0;
		/// pauses the output
		virtual void disable() = 0;
		/// adds a line to the write queue, which will be printed in another thread
		virtual void write(const std::string& line) = 0;
		/// outputs everything in the buffer at the time of this call
		virtual void flush() = 0;
	};
}

#endif // GHOST_INPUTCONTROLLER_HPP
