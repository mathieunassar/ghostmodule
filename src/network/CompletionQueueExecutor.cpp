#include "../../include/internal/network/CompletionQueueExecutor.hpp"
#include <iostream>

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
	//stop();
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
