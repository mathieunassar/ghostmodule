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

#ifndef GHOST_INTERNAL_SCHEDULEDEXECUTOR_HPP
#define GHOST_INTERNAL_SCHEDULEDEXECUTOR_HPP

#include <ghost/module/ScheduledExecutor.hpp>
#include <vector>
#include <chrono>
#include <future>
#include <functional>
#include <atomic>

namespace ghost
{
namespace internal
{
/**
 *	Executors can push tasks to the thread pool when configured by a user.
 *	TODO move out
 */
class Executor
{
public:
	virtual ~Executor() = default;
	virtual bool update() = 0;
	virtual void stop() = 0;
};

class ThreadPool;

/**
 *	An executor that can schedule commands to run after a given delay,
 *	or to execute periodically.
 */
class ScheduledExecutor : public Executor, public ghost::ScheduledExecutor
{
public:
	ScheduledExecutor(ThreadPool* threadPool);
	
	// From ghost::ScheduledExecutor
	void stop() override;
	void scheduleAtFixedRate(const std::function<void()>& task, const std::chrono::milliseconds& rate) override;

	/// Enqueues new tasks - called by the thread pool that created the executor.
	bool update() override;

private:
	ThreadPool* _threadPool;
	std::mutex _mutex;

	struct ScheduledTask
	{
		std::function<void()> task;
		std::future<void> lastFuture;
		std::chrono::steady_clock::time_point lastStart;
		std::chrono::milliseconds rate;
	};
	std::vector<ScheduledTask> _scheduledTasks;
	std::atomic_bool _enable{true};
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_SCHEDULEDEXECUTOR_HPP
