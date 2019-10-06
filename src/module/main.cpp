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

#include <ghost/module/Module.hpp>
#include <iostream>
#include <sstream>

class TestModule : public ghost::Module
{
	bool init() override
	{
		initializeConsole();
		getConsole()->setPromptFormat("$u>");
		getConsole()->setInputMode(ghost::InputMode::SEQUENTIAL);

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
