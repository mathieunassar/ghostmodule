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

#ifndef GHOST_INTERNAL_THREADPOOL_HPP
#define GHOST_INTERNAL_THREADPOOL_HPP

#include <BlockingQueue.hpp>
#include <atomic>
#include <future>
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
class ThreadPool
{
public:
	/**
	 *	@param threadsCount number of threads to manage in this pool.
	 */
	ThreadPool(size_t threadsCount);
	~ThreadPool();

	/**
	 *	Starts the worker threads in the pool.
	 *	If tasks have already been enqueued through calls to "execute",
	 *	then their execution begins.
	 *	If all the workers cannot be started, then stop is called and the
	 *	method returns false.
	 *	@return true if all the workers started successfully, false otherwise.
	 */
	bool start();
	/**
	 *	Stops all the execution in the thread pool.
	 *	If "joinThreads" is set to true, then the method will block
	 *	until all the worker threads have joined.
	 *	@param joinThreads	set to true to join the worker threads.
	 */
	void stop(bool joinThreads);
	/**
	 *	Enqueues a task that will be executed by a worker in this pool.
	 *	The returned promise will be fulfilled upon completion of the task.
	 */
	template <typename Callable>
	auto execute(Callable&& callable) -> std::future<typename std::result_of<Callable()>::type>;
	/**
	 *	Creates an executor that can schedule tasks to the thread pool.
	 *	@return the created executor that can schedule tasks.
	 */
	std::shared_ptr<ScheduledExecutor> makeScheduledExecutor();

private:
	/// The worker function, run by all the threads
	void worker();
	/// Checks the executors for new scheduled tasks.
	void updateExecutors();

	std::vector<std::thread> _threads;
	std::mutex _mutex;
	std::atomic_bool _enable = true;
	BlockingQueue<std::function<void(void)>> _queue;
	std::vector<std::shared_ptr<Executor>> _executors;
};

///// Template definition /////

template <typename Callable>
auto ThreadPool::execute(Callable&& callable) -> std::future<typename std::result_of<Callable()>::type>
{
	using ReturnType = typename std::result_of<Callable()>::type;

	// If the thread pool is stopped, don't execute anything
	if (!_enable)
	{
		return std::future<ReturnType>();
	}

	auto ptask = std::make_shared<std::packaged_task<ReturnType()>>(callable);
	std::future<ReturnType> res = ptask->get_future();
	_queue.push([ptask]() { (*ptask)(); });
	return res;
}

} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_THREADPOOL_HPP
