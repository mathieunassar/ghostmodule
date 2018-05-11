#include <iostream>
#include <sstream>

#include "../include/Module.hpp"

class TestModule : public Ghost::Module
{
	bool init() override
	{
		initializeConsole();
		getConsole()->setPromptFormat("$u>");
		getConsole()->setInputMode(Ghost::InputController::SEQUENTIAL);

		std::cout << " _______ _      _  ______  ________ _________" << std::endl;
		std::cout << "|  _____ |______| |      | |_______     |" << std::endl;
		std::cout << "|______| |      | |______| _______|     |" << std::endl;
		getConsole()->flush();

		return true;
	}

	bool run() override
	{
		sleepMillisecond(50);
		return true;
	}

	void dispose() override
	{
		std::cout << "Module disposed" << std::endl;
	}
};

int main()
{
	TestModule module;
	module.start();

	return 0;
}
