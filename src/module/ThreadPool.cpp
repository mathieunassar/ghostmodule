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

#include "ThreadPool.hpp"

using namespace ghost::internal;

ThreadPool::ThreadPool(size_t threadsCount) : _threadsCount(threadsCount)
{
}

ThreadPool::~ThreadPool()
{
	stop(true);
}

bool ThreadPool::start()
{
	std::unique_lock<std::mutex> lock(_poolMutex);

	// Check that the pool size is positive and that it has not started yet
	if (!_threads.empty()) return false;

	_enable = true;
	for (size_t i = 0; i < _threadsCount; ++i)
	{
		auto thread = std::thread(&ThreadPool::worker, this);
		_threads[thread.get_id()] = std::move(thread);
	}

	return true;
}

void ThreadPool::stop(bool joinThreads)
{
	std::unique_lock<std::mutex> lock(_executorsMutex);
	std::vector<std::shared_ptr<Executor>> executors = _executors;
	lock.unlock();

	// Stop the executors - this will wait that their tasks complete
	for (auto& executor : executors) executor->stop();

	// Stop tasks execution and join the workers
	// No need to lock the _poolMutex since the threads will not use shared data after _enable is false
	_enable = false;
	if (joinThreads)
	{
		for (auto& thread : _threads)
			if (thread.second.joinable()) thread.second.join();
		for (auto& thread : _threadsToJoin)
			if (thread.joinable()) thread.join();
	}
	_executors.clear();
}

void ThreadPool::resize(size_t newThreadsCount)
{
	std::unique_lock<std::mutex> lock(_poolMutex);
	_threadsCount = newThreadsCount;
	lock.unlock();

	checkThreadsCount();
}

size_t ThreadPool::size() const
{
	std::unique_lock<std::mutex> lock(_poolMutex);
	return _threads.size();
}

std::shared_ptr<ghost::ScheduledExecutor> ThreadPool::makeScheduledExecutor()
{
	std::unique_lock<std::mutex> lock(_executorsMutex);
	auto executor = std::make_shared<ScheduledExecutor>(this);
	_executors.push_back(executor);
	return executor;
}

bool ThreadPool::work()
{
	std::function<void()> task;
	bool taskGetResult = _queue.tryGetAndPop(std::chrono::milliseconds(1), task);

	if (taskGetResult && task)
	{
		// If a task was gotten, execute it
		task();
	}

	updateExecutors();
	checkThreadsCount();
	return true;
}

void ThreadPool::yield(const std::chrono::steady_clock::duration& minimumDuration)
{
	auto deadline = std::chrono::steady_clock::now() + minimumDuration;
	bool continueYielding = work();
	auto now = std::chrono::steady_clock::now();
	while (now < deadline && continueYielding)
	{
		work();
		continueYielding = _enable;
		now = std::chrono::steady_clock::now();
	}
}

bool ThreadPool::enabled() const
{
	return _enable;
}

void ThreadPool::enqueue(std::function<void(void)>&& task)
{
	_queue.push(task);
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

		// Manage the pool's size
		bool thisThreadEnable = checkThreadsCount();
		if (!thisThreadEnable) break;
	}
}

void ThreadPool::updateExecutors()
{
	std::unique_lock<std::mutex> lock(_executorsMutex);

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

bool ThreadPool::checkThreadsCount()
{
	std::unique_lock<std::mutex> lock(_poolMutex);

	// the pool ended or did not start yet, nothing to do
	if (!_enable) return false;

	// 1. Join threads waiting to finish
	for (auto& thread : _threadsToJoin)
		if (thread.joinable()) thread.join();
	_threadsToJoin.clear();

	// 1. Check if this thread must finish
	if (_threads.size() > _threadsCount)
	{
		// If the thread id is not in the map, it is the main thread, don't join it!!
		if (_threads.find(std::this_thread::get_id()) == _threads.end()) return true;

		// this thread finishes: move the thread from the map to the list of threads to join
		_threadsToJoin.push_back(std::move(_threads[std::this_thread::get_id()]));
		_threads.erase(std::this_thread::get_id());
		return false;
	}
	else if (_threads.size() < _threadsCount)
	{
		// Start as many threads as the user wants
		for (size_t i = _threads.size(); i < _threadsCount; ++i)
		{
			auto thread = std::thread(&ThreadPool::worker, this);
			_threads[thread.get_id()] = std::move(thread);
		}
	}

	return true;
}
