/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ConsoleDeviceWindows.hpp"

#include <iostream>
#include <thread>

using namespace ghost::internal;

ConsoleDeviceWindows::ConsoleDeviceWindows() : _enable(false), _mode(DeviceMode::IDLE)
{
}

bool ConsoleDeviceWindows::start()
{
	_enable = true;

	_hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (_hStdin == INVALID_HANDLE_VALUE) return false;

	if (!GetConsoleMode(_hStdin, &_originalConsoleMode)) return false;

	DWORD newMode = _originalConsoleMode | ENABLE_WINDOW_INPUT;
	if (!SetConsoleMode(_hStdin, newMode)) return false;

	return true;
}

void ConsoleDeviceWindows::stop()
{
	_enable = false;
	SetConsoleMode(_hStdin, _originalConsoleMode);
}

bool ConsoleDeviceWindows::setConsoleMode(ConsoleMode mode)
{
	DWORD consoleMode = 0;
	DWORD targetMode = consoleMode;

	if (!GetConsoleMode(_hStdin, &consoleMode)) return false;

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
	_mode = DeviceMode::AWAIT_INPUT;

	bool gotInput = awaitInput([&]() { return _mode == DeviceMode::AWAIT_INPUT && _enable.load(); });
	if (!gotInput) // _enable is false
		return false;

	if (!ReadConsoleInput(_hStdin,    // input buffer handle
			      irInBuf,    // buffer to read into
			      128,	// size of read buffer
			      &cNumRead)) // number of records read
		return false;

	for (i = 0; i < cNumRead; i++)
	{
		if (irInBuf[i].EventType == KEY_EVENT			      // key event
		    && irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN // which is about "enter"/"return"
		    && !irInBuf[i].Event.KeyEvent.bKeyDown)		      // and the key was released
			return true;
	}
	return false;
}

bool ConsoleDeviceWindows::awaitInput(const std::function<bool()>& untilPredicate)
{
	while (untilPredicate())
	{
		DWORD number;
		if (GetNumberOfConsoleInputEvents(_hStdin, &number) &&
		    number == 0) // don't start to read if there is nothing to read
		{
			std::this_thread::sleep_for(
			    std::chrono::microseconds(1)); // but don't wait too long to stay reactive
			continue;
		}

		return true;
	}
	return false;
}

bool ConsoleDeviceWindows::read(std::string& output, bool secret)
{
	_mode = DeviceMode::READ;

	if (secret) setConsoleMode(ConsoleDevice::OUTPUT);

	bool gotInput = awaitInput([&]() { return _mode == DeviceMode::READ && _enable.load(); });
	if (!gotInput) // _enable is false
		return false;

	std::getline(std::cin, output);

	if (secret) setConsoleMode(ConsoleDevice::INPUT);

	_mode = DeviceMode::IDLE;
	return true;
}

bool ConsoleDeviceWindows::write(const std::string& text)
{
	printf("%s", text.c_str());
	fflush(stdout);
	return true;
}
