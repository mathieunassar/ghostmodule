#ifndef GHOST_INTERNAL_NETWORK_COMPLETIONQUEUEEXECUTOR_HPP
#define GHOST_INTERNAL_NETWORK_COMPLETIONQUEUEEXECUTOR_HPP

#include <functional>
#include <thread>
#include <list>

#include <grpcpp/completion_queue.h>

namespace ghost
{
	namespace internal
	{
		class CompletionQueueExecutor
		{
		public:
			CompletionQueueExecutor();
			CompletionQueueExecutor(grpc::CompletionQueue* completion);
			~CompletionQueueExecutor();

			void setCompletionQueue(std::unique_ptr<grpc::CompletionQueue> completion);
			grpc::CompletionQueue* getCompletionQueue();

			void start(size_t threadsCount);
			void stop();

		private:
			void handleRpcs();

			std::unique_ptr<grpc::CompletionQueue> _completionQueue;

			std::list<std::thread> _threadPool;
		};


		/**
		 * Tag information for the gRPC completion queue.
		 * @author	Mathieu Nassar
		 * @date	17.06.2018
		 */
		struct TagInfo
		{
			std::function<void(bool)>* processor;
			bool ok;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_COMPLETIONQUEUEEXECUTOR_HPP
