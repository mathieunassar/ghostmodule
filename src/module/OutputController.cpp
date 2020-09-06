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

#include "OutputController.hpp"

using namespace ghost::internal;

OutputController::OutputController(const std::shared_ptr<ThreadPool>& threadPool, std::shared_ptr<ConsoleDevice> device,
				   bool redirectStdCout)
    : _redirecter(nullptr)
    , _activeInputQueue(&_writeQueue1)
    , _activeOutputQueue(&_writeQueue1)
    , _threadPool(threadPool)
    , _device(device)
    , _consoleMode(ConsoleDevice::OUTPUT)
{
	if (redirectStdCout)
	{
		_redirecter = std::unique_ptr<ConsoleStream<>>(new ConsoleStream<>(
		    std::cout,
		    std::bind(&OutputController::stdcoutCallback, this, std::placeholders::_1, std::placeholders::_2)));
	}
}

OutputController::~OutputController()
{
	stop();
}

void OutputController::start()
{
	if (!_executor)
	{
		_executor = _threadPool->makeScheduledExecutor();
		_executor->scheduleAtFixedRate(std::bind(&OutputController::writerTask, this),
					       std::chrono::milliseconds(1));
	}
}

void OutputController::stop()
{
	if (_executor)
	{
		_executor->stop();
		_executor.reset();
	}
}

void OutputController::enable()
{
	std::unique_lock<std::mutex> lock(_modeLock);
	_consoleMode = ConsoleDevice::OUTPUT;
	_waitForOutput.notify_all();
}

void OutputController::disable()
{
	std::unique_lock<std::mutex> lock(_modeLock);
	_consoleMode = ConsoleDevice::INPUT;
}

void OutputController::write(const std::string& line)
{
	QueueElement<std::string> element;
	element.element = line;
	element.result = std::make_shared<std::promise<bool>>();

	std::unique_lock<std::mutex> lock(_writeQueueSwitchLock);
	_activeInputQueue->push(element);
}

void OutputController::flush()
{
	if (!_executor) return;

	// take the flush lock to avoid the swap again before the one queue empties
	std::unique_lock<std::mutex> lock(_flushLock);

	swapQueues(
	    &_activeInputQueue); // now write() will write in the second queue while the writer empties the first queue

	// While there are lines to write in the queue, execute the tasks
	while (_activeOutputQueue->size() != 0) writerTask();

	// now _activeOutputQueue is empty and can start writing the content of the other queue
	swapQueues(&_activeOutputQueue);
}

bool OutputController::isEnabled() const
{
	std::unique_lock<std::mutex> lock(_modeLock);
	return _consoleMode == ConsoleDevice::OUTPUT;
}

void OutputController::stdcoutCallback(const char* ptr, std::streamsize count)
{
	std::string str(ptr, (size_t)count);
	write(str);
}

void OutputController::swapQueues(BlockingQueue<QueueElement<std::string>>** queue)
{
	std::unique_lock<std::mutex> lock(_writeQueueSwitchLock);
	if (*queue == &_writeQueue1)
		*queue = &_writeQueue2;
	else
		*queue = &_writeQueue1;
}

void OutputController::writerTask()
{
	std::unique_lock<std::mutex> modeLock(_modeLock);

	// Only process the outputs if the mode is output
	bool isOutputMode = _waitForOutput.wait_for(modeLock, std::chrono::milliseconds(0),
						    [&]() { return _consoleMode == ConsoleDevice::OUTPUT; });
	if (!isOutputMode) return;

	std::unique_lock<std::mutex> queueSwitchLock(_writeQueueSwitchLock);
	BlockingQueue<QueueElement<std::string>>* queue =
	    _activeOutputQueue; // choose the queue with the lock in case of flush
	queueSwitchLock.unlock();

	QueueElement<std::string> entry;
	if (!queue->tryGetAndPop(std::chrono::milliseconds(0), entry)) return;

	_device->write(entry.element);
	// (idea) the promise could be used to know when the entry is effectively executed...
	entry.result->set_value(true);
}
