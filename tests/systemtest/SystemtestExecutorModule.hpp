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

#ifndef GHOST_TESTS_SYSTEMTESTEXECUTORMODULE_HPP
#define GHOST_TESTS_SYSTEMTESTEXECUTORMODULE_HPP

#include <ghost/module/Module.hpp>
#include <map>
#include <string>
#include <thread>

#include "Systemtest.hpp"

/*
SYSTEMTEST:
- module that can be parameterized with different tests:
	- stress connection gRPC -> starting a lot of clients and getting messages
	- stress/endurance connection gRPC -> pub sub with random sub disconnect + send a lot of messages!!
		store messages in data files?
	-> configurable for duration of test
*/
class SystemtestExecutorModule
{
public:
	bool initialize(const ghost::Module& module);
	bool run(const ghost::Module& module);
	void dispose(const ghost::Module& module);

	void registerSystemtest(const std::shared_ptr<Systemtest>& test);
	void executeSystemtest(const std::string& testName, const Systemtest::Parameters& testParams);
	void stopSystemtest();

private:
	std::shared_ptr<ghost::Logger> _logger;

	std::map<std::string, std::shared_ptr<Systemtest>> _tests;
	std::shared_ptr<Systemtest> _activeTest;
	std::thread _testThread;
};

#endif // GHOST_TESTS_SYSTEMTESTEXECUTORMODULE_HPP
