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

#include "Systemtest.hpp"

void Systemtest::Parameters::print() const
{
	std::cout << "test duration: " << duration.count() << " s" << std::endl;
	std::cout << "test command line: " << std::endl;
	for (const auto p : commandLine.getParametersMap())
	{
		std::cout << "\t[" << p.first << "] = " << p.second << std::endl;
	}
}

Systemtest::Systemtest(const std::shared_ptr<ghost::ThreadPool>& threadPool,
		       const std::shared_ptr<ghost::Logger>& logger)
    : _threadPool(threadPool), _logger(logger), _state(Systemtest::State::READY)
{
}

bool Systemtest::execute(const Systemtest::Parameters& params)
{
	std::string testName = getName();

	if (getState() != State::READY && getState() != State::FINISHED)
	{
		GHOST_ERROR(_logger) << "Systemtest '" << testName << "' is already running.";
		return false;
	}

	_parameters = params;

	bool testResult = true;

	/* Setup the test */
	GHOST_INFO(_logger) << "Executing system test '" << testName << "': setup...";
	setState(State::SETTING_UP);
	bool setupResult = false;
	try
	{
		setupResult = setUp();
	}
	catch (std::logic_error e)
	{
		GHOST_ERROR(_logger) << "caught test exception: " << e.what();
		setupResult = false;
	}
	EXPECT_TRUE(setupResult);

	if (setupResult)
	{
		/* Run the test */
		GHOST_INFO(_logger) << "Executing system test '" << testName << "': setup succeeded! Now executing...";
		setState(State::EXECUTING);
		bool executionResult = false;
		try
		{
			executionResult = run();
		}
		catch (std::logic_error e)
		{
			GHOST_ERROR(_logger) << "caught test exception: " << e.what();
			executionResult = false;
		}
		if (!executionResult)
		{
			GHOST_ERROR(_logger) << "Systemtest '" << testName << "': execution failed!";
			testResult = false;
			EXPECT_TRUE(executionResult);
		}
		else
			GHOST_INFO(_logger) << "Systemtest '" << testName << "': execution succeeded!";
	}
	else
	{
		GHOST_ERROR(_logger) << "Systemtest '" << testName << "': setup failed!";
		testResult = false;
	}

	/* Print summary and tear down */
	setState(State::TEARING_DOWN);
	printSummary();

	GHOST_INFO(_logger) << "Executing system test '" << testName << "': tearing down...";
	tearDown();
	GHOST_INFO(_logger) << "Executing system test '" << testName << "': tear down completed.";

	setState(State::FINISHED);
	return testResult;
}

void Systemtest::require(bool condition, bool fatal)
{
	EXPECT_TRUE(condition);
	if (!condition && fatal)
	{
		GHOST_ERROR(_logger) << "expectation failed, finshing the test.";
		setState(State::DISPOSING);
	}
}

void Systemtest::printSummary() const
{
	GHOST_INFO(_logger) << "======================= Test completed =======================";
	GHOST_INFO(_logger) << "Total duration: "
			    << std::chrono::duration_cast<std::chrono::milliseconds>(_endTime - _startTime).count()
			    << " ms.";
	onPrintSummary();
	GHOST_INFO(_logger) << "==============================================================";
}

void Systemtest::stop()
{
	setState(State::DISPOSING);
}

Systemtest::State Systemtest::getState() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _state;
}

const Systemtest::Parameters& Systemtest::getParameter() const
{
	return _parameters;
}

bool Systemtest::checkTestDuration() const
{
	if (_parameters.duration.count() == 0) return true;

	return std::chrono::steady_clock::now() < _startTime + _parameters.duration;
}

void Systemtest::setState(const Systemtest::State& state)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_state = state;

	if (state == State::EXECUTING) _startTime = std::chrono::steady_clock::now();

	if (state == State::DISPOSING || state == State::TEARING_DOWN) _endTime = std::chrono::steady_clock::now();
}
