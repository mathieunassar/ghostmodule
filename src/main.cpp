#include <iostream>
#include <sstream>

#include "../include/internal/ConsoleDeviceWindows.hpp"
#include "../include/Console.hpp"

int main()
{
	Ghost::Console console;
	console.setPrompt("Mathieu>");
	console.setInputMode(Ghost::InputController::SEQUENTIAL);
	console.start();

	std::cout << "Hello, World!" << std::endl;
	std::cout << "john " << std::flush;
	std::cout << "is not here" << std::endl;
	std::cout << "not here either!" << std::endl;

	Sleep(1000);
	std::cout << "enter your age: ";
	auto str = console.getLine();
	std::cout << "you are " << str << " years old!" << std::endl;

	std::cout << "enter your age again: ";
	str = console.getLine();
	std::cout << "you are " << str << " years old!" << std::endl;

	Sleep(5000);

	int count = 10;
	while (count > 0)
	{
		std::ostringstream oss;
		oss << "this is a test" << count << std::endl;
		console.write(oss.str());
		count--;
		Sleep(1000);
	}
	
	Sleep(10000);

	console.stop();
	std::cout << "finished";

	return 0;
}
