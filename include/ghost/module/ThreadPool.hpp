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

#ifndef GHOST_MODULE_THREADPOOL_HPP
#define GHOST_MODULE_THREADPOOL_HPP

#include <ghost/module/ScheduledExecutor.hpp>
#include <memory>

namespace ghost
{
/**
 *	Executes tasks in a pool of threads.
 */
class ThreadPool
{
public:
	virtual ~ThreadPool() = default;
	/**
	 *	Starts the worker threads in the pool.
	 *	If tasks have already been enqueued through calls to "execute",
	 *	then their execution begins.
	 *	If all the workers cannot be started, then stop is called and the
	 *	method returns false.
	 *	@return true if all the workers started successfully, false otherwise.
	 */
	virtual bool start() = 0;
	/**
	 *	Stops all the execution in the thread pool.
	 *	If "joinThreads" is set to true, then the method will block
	 *	until all the worker threads have joined.
	 *	@param joinThreads	set to true to join the worker threads.
	 */
	virtual void stop(bool joinThreads) = 0;
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
	virtual std::shared_ptr<ghost::ScheduledExecutor> makeScheduledExecutor() = 0;

protected:
	virtual bool enabled() const = 0;
	virtual void enqueue(std::function<void(void)>&& task) = 0;
};

///// Template definition /////

template <typename Callable>
auto ThreadPool::execute(Callable&& callable) -> std::future<typename std::result_of<Callable()>::type>
{
	using ReturnType = typename std::result_of<Callable()>::type;

	// If the thread pool is stopped, don't execute anything
	if (!enabled()) return std::future<ReturnType>();

	auto ptask = std::make_shared<std::packaged_task<ReturnType()>>(callable);
	std::future<ReturnType> res = ptask->get_future();
	enqueue([ptask]() { (*ptask)(); });
	return res;
}
} // namespace ghost

#endif // GHOST_MODULE_THREADPOOL_HPP
