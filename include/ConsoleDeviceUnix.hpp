#ifndef GHOST_LOCALCONSOLE_UNIX_HPP
#define GHOST_LOCALCONSOLE_UNIX_HPP

#include "LocalConsole.hpp"

namespace Ghost
{
	/**
	*	Implementation of a {@ref LocalConsole} for Windows.
	*/
	class LocalConsoleUnix : public LocalConsole
	{
	public:
		bool setConsoleMode(ConsoleMode mode) override;
	};
}

#endif // GHOST_LOCALCONSOLE_UNIX_HPP
