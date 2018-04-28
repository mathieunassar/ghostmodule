#ifndef GHOST_LOCALCONSOLE_WINDOWS_HPP
#define GHOST_LOCALCONSOLE_WINDOWS_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "LocalConsole.hpp"

namespace Ghost
{
	/**
	*	Implementation of a {@ref LocalConsole} for Windows.
	*/
	class LocalConsoleWindows : public LocalConsole
	{
	public:
		bool setConsoleMode(ConsoleMode mode) override;
	};
}

#endif // GHOST_LOCALCONSOLE_WINDOWS_HPP
