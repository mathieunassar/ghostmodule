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

#ifndef GHOST_INTERNAL_NETWORK_COMPLETIONQUEUEEXECUTOR_HPP
#define GHOST_INTERNAL_NETWORK_COMPLETIONQUEUEEXECUTOR_HPP

#include <grpcpp/completion_queue.h>
#include <functional>
#include <ghost/module/ThreadPool.hpp>
#include <list>

namespace ghost
{
namespace internal
{
/**
 *	Starts concurrently listening to RPCs with the help of the provided
 *	ghost::ThreadPool.
 *	Manages the gRPC completion queue and processes tags that have been updated.
 */
class CompletionQueueExecutor
{
public:
	CompletionQueueExecutor(const std::shared_ptr<ghost::ThreadPool>& threadPool);
	CompletionQueueExecutor(grpc::CompletionQueue* completion,
				const std::shared_ptr<ghost::ThreadPool>& threadPool);
	~CompletionQueueExecutor();

	void setCompletionQueue(std::unique_ptr<grpc::CompletionQueue> completion);
	grpc::CompletionQueue* getCompletionQueue();

	void start(size_t threadsCount);
	void stop();

private:
	void handleRpcs();

	std::unique_ptr<grpc::CompletionQueue> _completionQueue;
	std::shared_ptr<ghost::ThreadPool> _threadPool;
	std::list<std::shared_ptr<ghost::ScheduledExecutor>> _executors;
	std::atomic_bool _completionQueueShutdown{true};
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
} // namespace internal
} // namespace ghost

#endif // GHOST_INTERNAL_NETWORK_COMPLETIONQUEUEEXECUTOR_HPP
