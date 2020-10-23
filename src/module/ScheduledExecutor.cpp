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

#include "ScheduledExecutor.hpp"
#include "ThreadPool.hpp"

using namespace ghost::internal;

ScheduledExecutor::ScheduledExecutor(ThreadPool* threadPool) : _threadPool(threadPool)
{
}

void ScheduledExecutor::stop()
{
	_enable = false;

	std::unique_lock<std::mutex> lock(_mutex);
	auto tasks = std::move(_scheduledTasks);
	lock.unlock();

	for (auto& t : tasks)
	{
		if (t->lastFuture.valid())
		{
			bool waitForFuture = true;
			while (waitForFuture)
			{
				auto status = t->lastFuture.wait_for(std::chrono::milliseconds(0));
				waitForFuture = status != std::future_status::ready;
				if (waitForFuture) _threadPool->yield(std::chrono::milliseconds(1));
			}

			t->lastFuture.get();
		}
	}
}

void ScheduledExecutor::scheduleAtFixedRate(const std::function<void()>& task, const std::chrono::milliseconds& rate)
{
	std::unique_lock<std::mutex> lock(_mutex);

	auto t = std::make_shared<ScheduledTask>();
	t->lastStart = std::chrono::steady_clock::now();
	t->rate = rate;
	t->task = task;
	t->taskCompleted = false;
	t->lastFuture = _threadPool->execute(std::bind(&ScheduledExecutor::scheduledTaskExecutor, this, t));

	_scheduledTasks.push_back(t);
}

void ScheduledExecutor::scheduledTaskExecutor(const std::shared_ptr<ScheduledTask>& task)
{
	task->task();
	std::unique_lock<std::mutex> lock(_mutex);
	task->taskCompleted = true;
}

bool ScheduledExecutor::update()
{
	std::unique_lock<std::mutex> lock(_mutex);
	// Return false if the executor was stopped
	if (!_enable) return false;

	auto now = std::chrono::steady_clock::now();
	for (auto& t : _scheduledTasks)
	{
		if (t->taskCompleted && now >= t->lastStart + t->rate)
		{
			t->lastStart = now;
			t->taskCompleted = false;
			t->lastFuture =
			    _threadPool->execute(std::bind(&ScheduledExecutor::scheduledTaskExecutor, this, t));
		}
	}
	return true;
}
