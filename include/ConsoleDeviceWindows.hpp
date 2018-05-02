#ifndef GHOST_CONSOLEDEVICE_WINDOWS_HPP
#define GHOST_CONSOLEDEVICE_WINDOWS_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "ConsoleDevice.hpp"

namespace Ghost
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
	};
}

#endif // GHOST_CONSOLEDEVICE_WINDOWS_HPP
