#include <iostream>
#include <sstream>

#include "../include/Module.hpp"

class TestModule : public Ghost::Module
{
	bool init() override
	{
		initializeConsole();
		getConsole()->setPrompt("Mathieu>");
		getConsole()->setInputMode(Ghost::InputController::SEQUENTIAL);

		std::cout << " _______ _      _  ______  ________ _________" << std::endl;
		std::cout << "|  _____ |______| |      | |_______     |" << std::endl;
		std::cout << "|______| |      | |______| _______|     |" << std::endl;

		return true;
	}

	bool run() override
	{
		sleepMillisecond(100000);
		return false;
	}
};

int main()
{
	TestModule module;
	module.start();

	return 0;
}
