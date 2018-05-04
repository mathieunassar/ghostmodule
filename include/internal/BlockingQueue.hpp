#ifndef GHOST_INTERNAL_BLOCKINGQUEUE_HPP
#define GHOST_INTERNAL_BLOCKINGQUEUE_HPP

#include <mutex>
#include <condition_variable>
#include <deque>
#include <future>
#include <memory>

namespace Ghost
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

		private:
			std::mutex _mutex;
			std::condition_variable _condition;
			std::deque<QueueElement<T>> _queue;
		};
	}
}

#endif //GHOST_INTERNAL_BLOCKINGQUEUE_HPP
