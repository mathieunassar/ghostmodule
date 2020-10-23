/*
 * Copyright 2020 Mathieu Nassar
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

#ifndef GHOST_TESTS_SYSTEMTEST_HPP
#define GHOST_TESTS_SYSTEMTEST_HPP

#include <gtest/gtest.h>

#include <chrono>
#include <ghost/module/CommandLine.hpp>
#include <ghost/module/Logger.hpp>
#include <ghost/module/ThreadPool.hpp>
#include <memory>
#include <mutex>
#include <string>

/**
 *	Base class for system tests.
 */
class Systemtest
{
public:
	struct Parameters
	{
		// the duration of the test if applicable (zero if no time constraints or infinite time)
		std::chrono::seconds duration;
		// the full command line for unmapped parameters
		ghost::CommandLine commandLine;

		void print() const;
	};

	enum class State
	{
		READY,
		SETTING_UP,
		EXECUTING,
		DISPOSING,
		FINISHED,
		TEARING_DOWN
	};

	Systemtest(const std::shared_ptr<ghost::ThreadPool>& threadPool, const std::shared_ptr<ghost::Logger>& logger);
	virtual ~Systemtest() = default;

	bool execute(const Systemtest::Parameters& params);
	void stop();
	State getState() const;

	virtual std::string getName() const = 0;

protected:
	const Parameters& getParameter() const;
	bool checkTestDuration() const;

	/// Configures the test with the input provided in the console
	virtual bool setUp()
	{
		return true;
	}
	/// Deinitializes the test if necessary
	virtual void tearDown()
	{
	}
	/// Executes the test prrogram.
	virtual bool run() = 0;

	void require(bool condition, bool fatal = true);
	void printSummary() const;
	virtual void onPrintSummary() const
	{
	}

	std::shared_ptr<ghost::ThreadPool> _threadPool;
	std::shared_ptr<ghost::Logger> _logger;

private:
	void setState(const State& state);

	std::chrono::steady_clock::time_point _startTime;
	std::chrono::steady_clock::time_point _endTime;
	Parameters _parameters;
	State _state;
	mutable std::mutex _mutex;
};

#endif // GHOST_TESTS_SYSTEMTEST_HPP
