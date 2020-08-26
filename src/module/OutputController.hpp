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

#ifndef GHOST_INTERNAL_OUTPUTCONTROLLER_HPP
#define GHOST_INTERNAL_OUTPUTCONTROLLER_HPP

#include <condition_variable>
#include <mutex>

#include "BlockingQueue.hpp"
#include "ConsoleDevice.hpp"
#include "ConsoleStream.hpp"
#include "ThreadPool.hpp"

namespace ghost
{
namespace internal
{
/**
 *	Implementation of the OutputController.
 */
class OutputController
{
public:
	OutputController(const std::shared_ptr<ThreadPool> &threadPool, std::shared_ptr<ConsoleDevice> device,
			 bool redirectStdCout = true);
	~OutputController();

	void start();
	void stop();
	void enable();
	void disable();
	void write(const std::string &line);
	void flush();
	bool isEnabled() const;

private:
	/* redirect std::cout stuff */
	void stdcoutCallback(const char *ptr, std::streamsize count);
	std::unique_ptr<ConsoleStream<>> _redirecter;

	/* Write queue - double buffered for efficient flushing */
	void swapQueues(BlockingQueue<QueueElement<std::string>> **queue);
	BlockingQueue<QueueElement<std::string>> _writeQueue1;
	BlockingQueue<QueueElement<std::string>> _writeQueue2;
	BlockingQueue<QueueElement<std::string>> *_activeInputQueue;  // write method fills this queue
	BlockingQueue<QueueElement<std::string>> *_activeOutputQueue; // writer writes from this queue
	std::mutex _writeQueueSwitchLock;
	std::mutex _flushLock;

	/* thread stuff */
	std::shared_ptr<ThreadPool> _threadPool;
	std::shared_ptr<ghost::ScheduledExecutor> _executor;
	void writerThread();
	std::condition_variable _waitForOutput;
	mutable std::mutex _modeLock;

	/* state */
	std::shared_ptr<ConsoleDevice> _device;
	ConsoleDevice::ConsoleMode _consoleMode;
};
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_OUTPUTCONTROLLER_HPP
