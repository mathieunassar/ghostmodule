#include "../include/LocalConsoleUnix.hpp"

#include <termios.h>
#include <unistd.h>

using namespace Ghost;

bool LocalConsoleUnix::setConsoleMode(ConsoleMode mode)
{
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);

	if (mode == INPUT)
		oldt.c_lflag |= ECHO;
	else
		oldt.c_lflag &= ~ECHO;

	return tcsetattr(STDIN_FILENO, TCSANOW, &oldt) == 0;
}
