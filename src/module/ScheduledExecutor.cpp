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

#include "ScheduledExecutor.hpp"
#include "ThreadPool.hpp"

using namespace ghost::internal;

ScheduledExecutor::ScheduledExecutor(ThreadPool* threadPool) : _threadPool(threadPool)
{
}

void ScheduledExecutor::stop()
{
	_enable = false;
}

void ScheduledExecutor::scheduleAtFixedRate(const std::function<void()>& task, const std::chrono::milliseconds& rate)
{
	ScheduledTask t;
	t.lastStart = std::chrono::steady_clock::now();
	t.rate = rate;
	t.task = task;
	t.lastFuture = _threadPool->execute(task);
	_scheduledTasks.push_back(std::move(t));
}

bool ScheduledExecutor::update()
{
	// Return false if the executor was stopped
	if (!_enable) return false;

	auto now = std::chrono::steady_clock::now();
	for (auto& t : _scheduledTasks)
	{
		if (t.lastFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready &&
		    now >= t.lastStart + t.rate)
		{
			t.lastStart = now;
			t.lastFuture = _threadPool->execute(t.task);
		}
	}
	return true;
}
