#include "../include/ConsoleDeviceWindows.hpp"
#include <strsafe.h>

using namespace Ghost;

ConsoleDeviceWindows::ConsoleDeviceWindows()
{

}

bool ConsoleDeviceWindows::start()
{
	_hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (_hStdin == INVALID_HANDLE_VALUE)
		return false;

	if (!GetConsoleMode(_hStdin, &_originalConsoleMode))
		return false;

	DWORD newMode = _originalConsoleMode | ENABLE_WINDOW_INPUT;
	if (!SetConsoleMode(_hStdin, newMode))
		return false;

	return true;
}

VOID ErrorExit(LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

}

bool ConsoleDeviceWindows::setConsoleMode(ConsoleMode mode)
{
	DWORD consoleMode = 0;
	DWORD targetMode = consoleMode;

	if (!GetConsoleMode(_hStdin, &consoleMode))
		return false;

	if (mode == INPUT)
		targetMode = consoleMode | ENABLE_ECHO_INPUT & (~ENABLE_WINDOW_INPUT);
	else
		targetMode = consoleMode & (~ENABLE_ECHO_INPUT) | ENABLE_WINDOW_INPUT;

	return SetConsoleMode(_hStdin, targetMode);
}

bool ConsoleDeviceWindows::awaitInputMode()
{
	DWORD cNumRead, i;
	INPUT_RECORD irInBuf[128];

	while (true)
	{
		if (!ReadConsoleInput(
			_hStdin,      // input buffer handle 
			irInBuf,     // buffer to read into 
			128,         // size of read buffer 
			&cNumRead)) // number of records read 
			return false;

		for (i = 0; i < cNumRead; i++)
		{
			if (irInBuf[i].EventType == KEY_EVENT							// key event
				&& irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN	// which is about "enter"/"return"
				&& !irInBuf[i].Event.KeyEvent.bKeyDown)						// and the key was released
			{
				return true;
			}
		}
	}

	return false; // dead code...
}

void ConsoleDeviceWindows::stop()
{
	SetConsoleMode(_hStdin, _originalConsoleMode);
}
