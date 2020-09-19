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

#ifndef GHOST_MODULE_SCHEDULEDEXECUTOR_HPP
#define GHOST_MODULE_SCHEDULEDEXECUTOR_HPP

#include <chrono>
#include <functional>

namespace ghost
{
/**
 *	An executor that can schedule commands to run after a given delay,
 *	or to execute periodically.
 *
 *	The ghost::ScheduledExecutor uses the standard chrono library (specifically, a steady clock)
 *	to measure time intervals. Scheduling tasks with specific rates does therefore not
 *	provide any real-time guarantee.
 */
class ScheduledExecutor
{
public:
	virtual ~ScheduledExecutor() = default;
	/**
	 *	Stops scheduling new tasks after schedule operations were called.
	 *	This method does not cancel the execution of already submitted tasks.
	 *	This method does not block and does not wait until completion of scheduled tasks.
	 */
	virtual void stop() = 0;
	/**
	 *	Creates a task in the attached thread pool that begins now,
	 *	and that is repeated with the given period until the executor is stopped.
	 *	If the task has not completed at the end of the period, the next task is delayed.
	 *	Calling this method more than once does not stop scheduling tasks from the previous calls.
	 *
	 *	@param task	task to schedule and repeat.
	 *	@param rate	period in milliseconds between two starts of the execution of the task.
	 */
	virtual void scheduleAtFixedRate(const std::function<void()>& task, const std::chrono::milliseconds& rate) = 0;
};
} // namespace ghost

#endif // GHOST_MODULE_SCHEDULEDEXECUTOR_HPP
