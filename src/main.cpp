#include "../include/ConsoleStream.hpp"
#include "../include/LocalConsoleWindows.hpp"
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

	Sleep(100000);

	console.stop();

	return 0;
}
