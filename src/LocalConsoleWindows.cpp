#include "../include/LocalConsoleWindows.hpp"

using namespace Ghost;

bool LocalConsoleWindows::setConsoleMode(ConsoleMode mode)
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD consoleMode = 0;
	DWORD targetMode = consoleMode;
	GetConsoleMode(hStdin, &consoleMode);

	if (mode == INPUT)
		targetMode = consoleMode | ENABLE_ECHO_INPUT;
	else
		targetMode = consoleMode & (~ENABLE_ECHO_INPUT);

	return SetConsoleMode(hStdin, targetMode);
}
