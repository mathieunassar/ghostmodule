#include "../include/internal/ConsoleDeviceWindows.hpp"
#include <thread>

using namespace ghost::internal;

ConsoleDeviceWindows::ConsoleDeviceWindows()
	: _enable(false)
{

}

bool ConsoleDeviceWindows::start()
{
	_enable = true;

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

bool ConsoleDeviceWindows::setConsoleMode(ConsoleMode mode)
{
	DWORD consoleMode = 0;
	DWORD targetMode = consoleMode;

	if (!GetConsoleMode(_hStdin, &consoleMode))
		return false;

	if (mode == INPUT)
		targetMode = consoleMode | ENABLE_ECHO_INPUT;
	else
		targetMode = consoleMode & (~ENABLE_ECHO_INPUT);

	return SetConsoleMode(_hStdin, targetMode);
}

bool ConsoleDeviceWindows::awaitInputMode()
{
	DWORD cNumRead, i;
	INPUT_RECORD irInBuf[128];

	while (_enable)
	{
		DWORD number;
		if (GetNumberOfConsoleInputEvents(_hStdin, &number) && number == 0) // don't start to read if there is nothing to read
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10)); // but don't wait too long to stay reactive
			continue;
		}

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
	_enable = false;
	SetConsoleMode(_hStdin, _originalConsoleMode);
}
