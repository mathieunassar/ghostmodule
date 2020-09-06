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

#include "ConsoleDeviceUnix.hpp"

#include <sys/select.h>

#include <iostream>
#include <thread>

using namespace ghost::internal;

ConsoleDeviceUnix::ConsoleDeviceUnix() : _enable(false), _mode(DeviceMode::IDLE)
{
}

bool ConsoleDeviceUnix::start()
{
	_enable = true;

	tcgetattr(STDIN_FILENO, &_referenceState);

	return true;
}

void ConsoleDeviceUnix::stop()
{
	_enable = false;

	setConsoleMode(ConsoleDevice::INPUT);
}

bool ConsoleDeviceUnix::setConsoleMode(ConsoleMode mode)
{
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);

	if (mode == INPUT)
		oldt.c_lflag |= ECHO;
	else
		oldt.c_lflag &= ~ECHO;

	return tcsetattr(STDIN_FILENO, TCSANOW, &oldt) == 0;
}

bool ConsoleDeviceUnix::awaitInputMode(const std::chrono::steady_clock::duration& timeout)
{
	_mode = DeviceMode::AWAIT_INPUT;

	bool gotInput = awaitInput([&]() { return _mode == DeviceMode::AWAIT_INPUT && _enable.load(); }, timeout);
	if (!gotInput) // _enable is false or the console is used for something else
		return false;

	std::string str;
	std::getline(std::cin, str);
	return _enable; // user pressed enter, hence the string is empty but there was something to read in the select
}

bool ConsoleDeviceUnix::read(std::string& output, bool secret)
{
	_mode = DeviceMode::READ;

	if (secret) setConsoleMode(ConsoleDevice::OUTPUT);

	bool gotInput = awaitInput([&]() { return _mode == DeviceMode::READ && _enable.load(); }, std::chrono::milliseconds(-1));
	if (!gotInput) // _enable is false or the console is used for something else
		return false;

	std::getline(std::cin, output);

	if (secret) setConsoleMode(ConsoleDevice::INPUT);

	_mode = DeviceMode::IDLE;
	return true;
}

bool ConsoleDeviceUnix::write(const std::string& text)
{
	printf("%s", text.c_str());
	fflush(stdout);
	return true;
}

bool ConsoleDeviceUnix::awaitInput(const std::function<bool()>& untilPredicate,
				   const std::chrono::steady_clock::duration& timeout)
{
	auto now = std::chrono::steady_clock::now();
	auto deadline = now + timeout;
	while (untilPredicate() && (timeout.count() < 0 || now < deadline))
	{
		fd_set fdr;
		FD_ZERO(&fdr);
		FD_SET(STDIN_FILENO, &fdr);

		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		int selectResult = select(STDIN_FILENO + 1, &fdr, NULL, NULL, &tv);
		if (selectResult == -1)
		{
			return false; // error!!
		}
		else if (selectResult == 0) // nothing to read
		{
			// return without waiting if the timeout is set to zero
			if (timeout.count() == 0) return false;

			std::this_thread::sleep_for(
			    std::chrono::milliseconds(1)); // but don't wait too long to stay reactive
			now = std::chrono::steady_clock::now();
			continue;
		}

		return true;
	}

	return false;
}
