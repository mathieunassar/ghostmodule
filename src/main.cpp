#include "../include/internal/ConsoleStream.hpp"
#include "../include/internal/ConsoleDeviceWindows.hpp"
#include "../include/Console.hpp"

void mycallback(const char *ptr, std::streamsize count)
{
	printf("%s", ptr);
}

int main()
{
	//redirect std::cout to the callback function
	Ghost::internal::ConsoleStream<> redirect(std::cout, mycallback);

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
