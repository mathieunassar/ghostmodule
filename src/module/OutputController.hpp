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

#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include "ConsoleDevice.hpp"
#include "ConsoleStream.hpp"
#include "BlockingQueue.hpp"
#include "ConsoleDevice.hpp"

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
			OutputController(std::shared_ptr<ConsoleDevice> device,
				bool redirectStdCout = true);
			~OutputController();

			void start();
			void stop();
			void enable();
			void disable();
			void write(const std::string& line);
			void flush();
			bool isEnabled() const;

		private:
			/* redirect std::cout stuff */
			void stdcoutCallback(const char *ptr, std::streamsize count);
			std::unique_ptr<ConsoleStream<>> _redirecter;

			/* Write queue - double buffered for efficient flushing */
			void swapQueues(BlockingQueue<QueueElement<std::string>>** queue);
			BlockingQueue<QueueElement<std::string>> _writeQueue1;
			BlockingQueue<QueueElement<std::string>> _writeQueue2;
			BlockingQueue<QueueElement<std::string>> *_activeInputQueue; // write method fills this queue
			BlockingQueue<QueueElement<std::string>> *_activeOutputQueue; // writer writes from this queue
			std::mutex _writeQueueSwitchLock;
			std::mutex _flushLock;

			/* thread stuff */
			void writerThread();
			bool awaitOutput();
			std::thread _writerThread;
			std::atomic<bool> _threadEnable;
			std::condition_variable _waitForOutput;
			mutable std::mutex _waitForOutputLock;

			/* state */
			std::shared_ptr<ConsoleDevice> _device;
			ConsoleDevice::ConsoleMode _consoleMode;
		};
	}
}

#endif // GHOST_INTERNAL_OUTPUTCONTROLLER_HPP
