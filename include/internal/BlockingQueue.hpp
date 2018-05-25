#ifndef GHOST_INTERNAL_BLOCKINGQUEUE_HPP
#define GHOST_INTERNAL_BLOCKINGQUEUE_HPP

#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <future>
#include <memory>

namespace ghost
{
	namespace internal
	{
		template<typename T>
		class QueueElement
		{
		public:
			T element;
			std::shared_ptr<std::promise<bool>> result;
		};

		// from https://stackoverflow.com/questions/12805041/c-equivalent-to-javas-blockingqueue
		template <typename T>
		class BlockingQueue
		{
		public:
			void push(const QueueElement<T>& value)
			{
				{
					std::unique_lock<std::mutex> lock(_mutex);
					_queue.push_front(value);
				}
				_condition.notify_one();
			}

			QueueElement<T> pop()
			{
				std::unique_lock<std::mutex> lock(_mutex);
				_condition.wait(lock, [=] { return !_queue.empty(); });
				QueueElement<T> rc(std::move(_queue.back()));
				_queue.pop_back();
				return rc;
			}

			bool tryPop(std::chrono::milliseconds timeout, QueueElement<T>& result)
			{
				std::unique_lock<std::mutex> lock(_mutex);
				if (size() == 0)
					_condition.wait_for(lock, timeout, [=] { return !_queue.empty(); });
	
				if (size() == 0)
					return false;

				result = std::move(_queue.back());
				_queue.pop_back();
				return true;
			}

			size_t size()
			{
				return _queue.size();
			}

		private:
			std::mutex _mutex;
			std::condition_variable _condition;
			std::deque<QueueElement<T>> _queue;
		};
	}
}

#endif //GHOST_INTERNAL_BLOCKINGQUEUE_HPP
