#ifndef GHOST_INTERNAL_CONSOLEDEVICE_UNIX_HPP
#define GHOST_INTERNAL_CONSOLEDEVICE_UNIX_HPP

#include <termios.h>
#include <unistd.h>
#include <atomic>

#include "ConsoleDevice.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		*	Implementation of a {@ref ConsoleDevice} for Windows.
		*/
		class ConsoleDeviceUnix : public ConsoleDevice
		{
		public:
			ConsoleDeviceUnix();

			bool start() override;

			bool setConsoleMode(ConsoleMode mode) override;

			bool awaitInputMode() override;

			void stop() override;

		private:
			termios _referenceState;
			std::atomic<bool> _enable;
		};
	}
}

#endif // GHOST_INTERNAL_CONSOLEDEVICE_UNIX_HPP
