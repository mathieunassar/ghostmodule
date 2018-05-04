#ifndef GHOST_INTERNAL_OUTPUTCONTROLLER_HPP
#define GHOST_INTERNAL_OUTPUTCONTROLLER_HPP

#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include "ConsoleStream.hpp"
#include "../OutputController.hpp"
#include "BlockingQueue.hpp"
#include "ConsoleDevice.hpp"

namespace Ghost
{
	namespace internal
	{
		/**
		 *	Implementation of the OutputController.
		 */
		class OutputController : public ::Ghost::OutputController
		{
		public:
			OutputController(bool redirectStdCout = true);

			void start() override;
			void stop() override;
			void enable() override;
			void disable() override;
			void write(const std::string& line) override;
			void flush() override;

		private:
			/* redirect std::cout stuff */
			void stdcoutCallback(const char *ptr, std::streamsize count);
			std::unique_ptr<ConsoleStream<>> _redirecter;

			/* Write queue - double buffered */
			void swapQueues(BlockingQueue<std::string>* queue);
			BlockingQueue<std::string> _writeQueue1;
			BlockingQueue<std::string> _writeQueue2;
			BlockingQueue<std::string> *_activeInputQueue; // write method fills this queue
			BlockingQueue<std::string> *_activeOutputQueue; // writer writes from this queue
			std::mutex _writeQueueSwitchLock;
			std::mutex _flushLock;

			/* thread stuff */
			void writerThread();
			bool awaitOutput();
			std::thread _writerThread;
			std::atomic<bool> _threadEnable;
			std::condition_variable _waitForOutput;
			std::mutex _waitForOutputLock;

			/* state */
			ConsoleDevice::ConsoleMode _consoleMode;
		};
	}
}

#endif // GHOST_INTERNAL_OUTPUTCONTROLLER_HPP
