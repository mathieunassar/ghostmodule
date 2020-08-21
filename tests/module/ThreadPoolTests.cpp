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

#include <gtest/gtest.h>

#include "../src/module/ThreadPool.hpp"

class ThreadPoolTests : public testing::Test
{
protected:
	void SetUp() override
	{
		
	}

	void TearDown() override
	{
	}

	void goToThreadPoolStartedState(size_t threadCount = 2)
	{
		_threadPool = std::make_shared<ghost::internal::ThreadPool>(threadCount);
		bool startResult = _threadPool->start();
		ASSERT_TRUE(startResult);
	}

	std::shared_ptr<ghost::internal::ThreadPool> _threadPool;

public:
	int testTaskThatReturns42()
	{
		return 42;
	}
};

TEST_F(ThreadPoolTests, Test_ThreadPool_startsAndStop_When_1ThreadIsStarted)
{
	goToThreadPoolStartedState(1);
}

TEST_F(ThreadPoolTests, Test_ThreadPool_doesnotStart_When_0ThreadIsStarted)
{
	_threadPool = std::make_shared<ghost::internal::ThreadPool>(0);
	bool startResult = _threadPool->start();
	ASSERT_FALSE(startResult);
}

TEST_F(ThreadPoolTests, Test_ThreadPool_startsAndStop_When_2ThreadIsStarted)
{
	goToThreadPoolStartedState();
}

TEST_F(ThreadPoolTests, Test_ThreadPool_executesLambdaTask_When_noReturnValueExpected)
{
	goToThreadPoolStartedState();
	bool valueChanged = false;
	auto future = _threadPool->execute([&valueChanged]() { valueChanged = true; });
	future.get();
	ASSERT_TRUE(valueChanged);
}

TEST_F(ThreadPoolTests, Test_ThreadPool_executesLambdaTask_When_valueIsExpected)
{
	goToThreadPoolStartedState();
	auto future = _threadPool->execute([]() { return 42; });
	ASSERT_EQ(future.get(), 42);
}

TEST_F(ThreadPoolTests, Test_ThreadPool_executesTask_When_valueIsExpected)
{
	goToThreadPoolStartedState();
	auto future = _threadPool->execute(std::bind(&ThreadPoolTests::testTaskThatReturns42, this));
	ASSERT_EQ(future.get(), 42);
}

TEST_F(ThreadPoolTests, Test_ThreadPool_executesTaskParallely_When_twoTasksAreCommanded)
{
	goToThreadPoolStartedState();
	std::atomic_bool allowedToComplete = false;
	int maxIterations = 100;
	auto future = _threadPool->execute([&]() {
		while (!allowedToComplete && maxIterations > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			--maxIterations;
		}
	});
	auto future2 = _threadPool->execute([&]() { allowedToComplete = true; });

	future.get();
	future2.get();
	ASSERT_TRUE(allowedToComplete);
	ASSERT_GE(maxIterations, 0);
}

TEST_F(ThreadPoolTests, Test_ScheduleExecutor_poolStops_When_executorIsStillActive)
{
	goToThreadPoolStartedState();
	auto executor = _threadPool->makeScheduledExecutor();

	int counter = 0;
	executor->scheduleAtFixedRate([&]() { counter++; }, std::chrono::milliseconds(1));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	_threadPool->stop(true);

	// no real assertion in this test, it would fail if the test takes forever (if stop does not stop)
	ASSERT_TRUE(true);
}

TEST_F(ThreadPoolTests, Test_ScheduleExecutor_pushesToPool_When_ok)
{
	goToThreadPoolStartedState();
	auto executor = _threadPool->makeScheduledExecutor();

	int counter = 0;
	executor->scheduleAtFixedRate([&]() { counter++; }, std::chrono::milliseconds(5));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	_threadPool->stop(true);
	std::cout << counter << std::endl;
	ASSERT_GE(counter, 15);
	ASSERT_LE(counter, 21);
}

TEST_F(ThreadPoolTests, Test_ScheduleExecutor_usesAllTheThreads_When_scheduleAtFixedRate)
{
	goToThreadPoolStartedState();
	auto executor = _threadPool->makeScheduledExecutor();

	std::map<std::string, int> threads;
	executor->scheduleAtFixedRate(
	    [&]() {
		    std::ostringstream oss;
		    oss << std::this_thread::get_id();
		    threads[oss.str()]++;
	    },
	    std::chrono::milliseconds(1));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	_threadPool->stop(true);
	ASSERT_EQ(threads.size(), 2);
}
