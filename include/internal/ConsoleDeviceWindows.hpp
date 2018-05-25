#ifndef GHOST_INTERNAL_CONSOLEDEVICE_WINDOWS_HPP
#define GHOST_INTERNAL_CONSOLEDEVICE_WINDOWS_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <atomic>

#include "ConsoleDevice.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		*	Implementation of a {@ref LocalConsole} for Windows.
		*/
		class ConsoleDeviceWindows : public ConsoleDevice
		{
		public:
			ConsoleDeviceWindows();

			bool start() override;

			bool setConsoleMode(ConsoleMode mode) override;

			bool awaitInputMode() override;

			void stop() override;

		private:
			HANDLE _hStdin;
			DWORD _originalConsoleMode;
			std::atomic<bool> _enable;
		};
	}
}

#endif // GHOST_INTERNAL_CONSOLEDEVICE_WINDOWS_HPP
