/*#include <windows.h>
#include <stdio.h>

HANDLE hStdin;
DWORD fdwSaveOldMode;

VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);

int main(VOID)
{
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter = 0;

	// Get the standard input handle. 

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");

	// Save the current input mode, to be restored on exit. 

	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");

	// Enable the window and mouse input events. 

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");

	// Loop to read and handle the next 100 input events. 

	while (counter++ <= 100)
	{
		// Wait for the events. 

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle 
			irInBuf,     // buffer to read into 
			128,         // size of read buffer 
			&cNumRead)) // number of records read 
			ErrorExit("ReadConsoleInput");

		// Dispatch the events to the appropriate handler. 

		for (i = 0; i < cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input 
				KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;
			default:
				break;
			}
		}
	}

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	return 0;
}

VOID ErrorExit(LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

VOID KeyEventProc(KEY_EVENT_RECORD ker)
{
	if (ker.wVirtualKeyCode == VK_RETURN && !ker.bKeyDown)
		printf("enter key pressed\n");
}*/


#include "../include/ConsoleStream.hpp"
#include "../include/ConsoleDeviceWindows.hpp"
#include "../include/Console.hpp"

void mycallback(const char *ptr, std::streamsize count)
{
	printf("%s", ptr);
}

int main()
{
	//redirect std::cout to the callback function
	Ghost::ConsoleStream<> redirect(std::cout, mycallback);

	std::cout << "Hello, World!" << std::endl;
	std::cout << "john " << std::flush;
	std::cout << "is not here" << std::endl;
	std::cout << "not here either!" << std::endl;

	Ghost::Console console;
	console.setPrompt("Mathieu>");
	console.setInputMode(Ghost::InputController::SEQUENTIAL);
	console.start();

	Sleep(1000);
	std::cout << "enter your age: ";
	auto str = console.getLine();
	std::cout << "you are " << str << " years old!" << std::endl;

	std::cout << "enter your age again: ";
	str = console.getLine();
	std::cout << "you are " << str << " years old!" << std::endl;
	
	Sleep(100000);

	console.stop();

	return 0;
}
