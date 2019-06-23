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
#include <thread>
#include <iostream>
#include <sys/select.h>

using namespace ghost::internal;

ConsoleDeviceUnix::ConsoleDeviceUnix()
	: _enable(false)
{

}

bool ConsoleDeviceUnix::start()
{
	_enable = true;

	tcgetattr(STDIN_FILENO, &_referenceState);
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

bool ConsoleDeviceUnix::awaitInputMode()
{
	fd_set fdr;
	FD_ZERO(&fdr);
	FD_SET(STDIN_FILENO, &fdr);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	while (_enable)
	{
		int selectResult = select(STDIN_FILENO + 1, &fdr, NULL, NULL, &tv);
		if (selectResult == -1)
		{
			return false; // error!!
		}
		else if (selectResult == 0) // nothing to read
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10)); // but don't wait too long to stay reactive
			continue;
		}
		else // something to read
		{
			std::string str;
			std::getline(std::cin, str);
			return _enable; // user pressed enter, hence the string is empty but there was something to read in the select
		}
	}

	return false;
}

void ConsoleDeviceUnix::stop()
{
	_enable = false;

	setConsoleMode(ConsoleDevice::INPUT);
}
