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

#include "CompletionQueueExecutor.hpp"

using namespace ghost::internal;

CompletionQueueExecutor::CompletionQueueExecutor()
{
}

CompletionQueueExecutor::CompletionQueueExecutor(grpc::CompletionQueue* completion)
	: _completionQueue(completion)
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
	for (size_t i = 0; i < 2; i++)
	{
		_threadPool.push_back(std::thread(&CompletionQueueExecutor::handleRpcs, this));
	}
}

void CompletionQueueExecutor::stop()
{
	if (_completionQueue)
		_completionQueue->Shutdown();
	
	for (auto& t : _threadPool)
	{
		if (t.joinable())
			t.join();
	}
}

void CompletionQueueExecutor::handleRpcs()
{
	TagInfo tag;
	while (true)
	{
		// Block waiting to read the next event from the completion queue. The
		// event is uniquely identified by its tag, which in this case is the
		// memory address of a CallData instance.
		// The return value of Next should always be checked. This return value
		// tells us whether there is any kind of event or cq_ is shutting down.
		bool nextSuccess = _completionQueue->Next((void**)&tag.processor, &tag.ok);
		if (!nextSuccess)
		{
			break;
		}

		(*tag.processor)(tag.ok);
	}
}
