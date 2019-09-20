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

#include "SystemtestExecutorModule.hpp"

#include <gtest/gtest.h>
#include "SystemtestCommand.hpp"
#include "StopSystemtestCommand.hpp"
#include "ConnectionStressTest.hpp"
#include "ConnectionMonkeyTest.hpp"

bool SystemtestExecutorModule::initialize(const ghost::Module& module)
{
	module.printGhostASCII("systemtest");
	_logger = module.getLogger();

	module.getInterpreter()->registerCommand(std::make_shared<SystemtestCommand>(this, module.getConsole()));
	module.getInterpreter()->registerCommand(std::make_shared<StopSystemtestCommand>(this));

	registerSystemtest(std::make_shared<ConnectionStressTest>(_logger));
	registerSystemtest(std::make_shared<ConnectionMonkeyTest>(_logger));

	GHOST_INFO(_logger) << "Systemtest executor initialized";
	return true;
}

bool SystemtestExecutorModule::run(const ghost::Module& module)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));

	return true;
}

void SystemtestExecutorModule::dispose(const ghost::Module& module)
{
	stopSystemtest();
	if (_testThread.joinable())
		_testThread.join();
}

void SystemtestExecutorModule::registerSystemtest(const std::shared_ptr<Systemtest>& test)
{
	_tests[test->getName()] = test;
}

void SystemtestExecutorModule::executeSystemtest(const std::string& testName, const Systemtest::Parameters& testParams)
{
	if (_activeTest && _activeTest->getState() != Systemtest::State::FINISHED)
	{
		GHOST_ERROR(_logger) << "systemtest '" << _activeTest->getName() << "' is still executing.";
		return;
	}

	if (_tests.find(testName) == _tests.end())
	{
		GHOST_ERROR(_logger) << "No systemtest named '" << testName << "' was registered.";
		return;
	}

	GHOST_INFO(_logger) << "Executing system test '" << testName << "' with the following parameters:";
	testParams.print();

	if (_testThread.joinable())
		_testThread.join();

	_activeTest = _tests[testName];
	_testThread = std::thread([&](){ _activeTest->execute(testParams); });
}

void SystemtestExecutorModule::stopSystemtest()
{
	if (_activeTest)
	{
		GHOST_INFO(_logger) << "Stopping system test '" << _activeTest->getName() << "'.";
		_activeTest->stop();

		if (_testThread.joinable())
			_testThread.join();

		_activeTest.reset();
	}
}
