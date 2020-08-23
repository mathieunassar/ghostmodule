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

#include "ThreadPool.hpp"

using namespace ghost::internal;

ThreadPool::ThreadPool(size_t threadsCount)
{
	_threads.resize(threadsCount);
}

ThreadPool::~ThreadPool()
{
	stop(true);
}

bool ThreadPool::start()
{
	// Check that the pool size is positive and that it has not started yet
	if (_threads.size() == 0 || _threads.front().joinable()) return false;

	_enable = true;
	for (size_t i = 0; i < _threads.size(); ++i) _threads[i] = std::thread(&ThreadPool::worker, this);

	return true;
}

void ThreadPool::stop(bool joinThreads)
{
	std::unique_lock<std::mutex> lock(_mutex);
	std::vector<std::shared_ptr<Executor>> executors = _executors;
	lock.unlock();

	// Stop the executors - this will wait that their tasks complete
	for (auto& executor : executors) executor->stop();

	// Stop tasks execution and join the workers
	_enable = false;
	if (joinThreads)
	{
		for (auto& thread : _threads)
		{
			if (thread.joinable()) thread.join();
		}
	}
	_executors.clear();
}

std::shared_ptr<ScheduledExecutor> ThreadPool::makeScheduledExecutor()
{
	std::unique_lock<std::mutex> lock(_mutex);
	auto executor = std::make_shared<ScheduledExecutor>(this);
	_executors.push_back(executor);
	return executor;
}

void ThreadPool::worker()
{
	while (_enable)
	{
		std::function<void()> task;
		bool taskGetResult = _queue.tryGetAndPop(std::chrono::milliseconds(1), task);

		// If a task was gotten, execute it
		if (taskGetResult && task) task();

		updateExecutors();
	}
}

void ThreadPool::updateExecutors()
{
	std::unique_lock<std::mutex> lock(_mutex);

	auto it = _executors.begin();
	while (it != _executors.end())
	{
		bool executorIsRunning = (*it)->update();

		// If the executor is not running anymore, delete it
		if (!executorIsRunning)
			it = _executors.erase(it);
		else
			++it;
	}
}
