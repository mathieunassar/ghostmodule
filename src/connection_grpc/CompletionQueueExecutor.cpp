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

#include "CompletionQueueExecutor.hpp"

using namespace ghost::internal;

CompletionQueueExecutor::CompletionQueueExecutor(const std::shared_ptr<ghost::ThreadPool>& threadPool)
    : _threadPool(threadPool)
{
}

CompletionQueueExecutor::CompletionQueueExecutor(grpc::CompletionQueue* completion,
						 const std::shared_ptr<ghost::ThreadPool>& threadPool)
    : _completionQueue(completion), _threadPool(threadPool)
{
}

CompletionQueueExecutor::~CompletionQueueExecutor()
{
	stop();
}

void CompletionQueueExecutor::setCompletionQueue(std::unique_ptr<grpc::CompletionQueue> completion)
{
	_completionQueue = std::move(completion);
}

grpc::CompletionQueue* CompletionQueueExecutor::getCompletionQueue()
{
	return _completionQueue.get();
}

void CompletionQueueExecutor::start(size_t threadsCount)
{
	for (size_t i = 0; i < threadsCount; i++)
	{
		auto executor = _threadPool->makeScheduledExecutor();
		executor->scheduleAtFixedRate(std::bind(&CompletionQueueExecutor::handleRpcs, this),
					      std::chrono::milliseconds(10));
		_executors.push_back(executor);
	}
}

void CompletionQueueExecutor::stop()
{
	if (_completionQueue) _completionQueue->Shutdown();

	while (!_completionQueueShutdown) _threadPool->yield(std::chrono::milliseconds(10));

	for (auto& t : _executors) t->stop();
}

void CompletionQueueExecutor::handleRpcs()
{
	while (true) // loop until there are no new tags to find
	{
		TagInfo tag;
		// Block waiting to read the next event from the completion queue. The
		// event is uniquely identified by its tag, which in this case is the
		// memory address of a CallData instance.
		// The return value of Next should always be checked. This return value
		// tells us whether there is any kind of event or cq_ is shutting down.
		auto now = std::chrono::system_clock::now();
		auto status = _completionQueue->AsyncNext((void**)&tag.processor, &tag.ok, now);

		// Update the state of the completion queue
		if (status == grpc::CompletionQueue::NextStatus::SHUTDOWN)
			_completionQueueShutdown = true;
		else
			_completionQueueShutdown = false;

		// Only pass this point if there is something to complete.
		if (status != grpc::CompletionQueue::NextStatus::GOT_EVENT) return;

		(*tag.processor)(tag.ok);
	}
}
