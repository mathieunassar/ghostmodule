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

#ifndef GHOST_INTERNAL_THREADPOOL_HPP
#define GHOST_INTERNAL_THREADPOOL_HPP

#include <BlockingQueue.hpp>
#include <atomic>
#include <future>
#include <ghost/module/ThreadPool.hpp>
#include <list>
#include <map>
#include <mutex>
#include <thread>
#include <vector>

#include "ScheduledExecutor.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Executes tasks in a pool of threads.
 */
class ThreadPool : public ghost::ThreadPool
{
public:
	/**
	 *	@param threadsCount number of threads to manage in this pool.
	 */
	ThreadPool(size_t threadsCount);
	~ThreadPool();

	bool start() override;
	void stop(bool joinThreads) override;
	void resize(size_t newThreadsCount) override;
	size_t size() const override;
	std::shared_ptr<ghost::ScheduledExecutor> makeScheduledExecutor() override;
	bool work() override;
	void yield(const std::chrono::steady_clock::duration& minimumDuration = std::chrono::milliseconds(1)) override;

protected:
	bool enabled() const override;
	void enqueue(std::function<void(void)>&& task) override;

private:
	/// The worker function, run by all the threads
	void worker();
	/// Checks the executors for new scheduled tasks.
	void updateExecutors();
	/// Checks if new threads must be spawned or if this thread must terminate
	bool checkThreadsCount();

	std::map<std::thread::id, std::thread> _threads;
	std::list<std::thread> _threadsToJoin;
	size_t _threadsCount{0};
	std::mutex _executorsMutex;
	mutable std::mutex _poolMutex;
	std::atomic_bool _enable{false};
	BlockingQueue<std::function<void(void)>> _queue;
	std::vector<std::shared_ptr<Executor>> _executors;
};

} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_THREADPOOL_HPP
