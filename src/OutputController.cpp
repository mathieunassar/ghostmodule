#include "../include/internal/OutputController.hpp"

using namespace ghost::internal;

OutputController::OutputController(bool redirectStdCout)
	: _redirecter(nullptr)
	, _activeInputQueue(&_writeQueue1)
	, _activeOutputQueue(&_writeQueue1)
	, _threadEnable(false)
	, _consoleMode(ConsoleDevice::OUTPUT)
{
	if (redirectStdCout)
	{
		_redirecter = std::unique_ptr<ConsoleStream<>>(new ConsoleStream<>(
			std::cout, std::bind(&OutputController::stdcoutCallback, this, std::placeholders::_1, std::placeholders::_2)));
	}
}

void OutputController::start()
{
	if (!_threadEnable)
	{
		_threadEnable = true;
		_writerThread = std::thread(&OutputController::writerThread, this);
	}
}

void OutputController::stop()
{
	_threadEnable = false;
	if (_writerThread.joinable())
		_writerThread.join();
}

void OutputController::enable()
{
	std::unique_lock<std::mutex> lock(_waitForOutputLock);
	_consoleMode = ConsoleDevice::OUTPUT;
	_waitForOutput.notify_all();
}

void OutputController::disable()
{
	std::unique_lock<std::mutex> lock(_waitForOutputLock);
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
	// take the flush lock to avoid the swap again before the one queue empties
	std::unique_lock<std::mutex> lock(_flushLock);

	swapQueues(_activeInputQueue); // now write() will write in the second queue while the writer empties the first queue

	while (_activeOutputQueue->size() != 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // wait 1ms
	}

	// now _activeOutputQueue is empty and can start writing the content of the other queue
	swapQueues(_activeOutputQueue);
}

void OutputController::stdcoutCallback(const char *ptr, std::streamsize count)
{
	std::string str(ptr, (size_t) count);
	write(str);
}

void OutputController::swapQueues(BlockingQueue<std::string>* queue)
{
	std::unique_lock<std::mutex> lock(_writeQueueSwitchLock);
	if (queue == &_writeQueue1)
		queue = &_writeQueue2;
	else
		queue = &_writeQueue1;
}

void OutputController::writerThread()
{
	while (_threadEnable)
	{
		if (!awaitOutput()) // wait until the console mode is input
			return;

		std::unique_lock<std::mutex> lock(_writeQueueSwitchLock);
		BlockingQueue<std::string>* queue = _activeOutputQueue; // choose the queue with the lock in case of flush
		lock.unlock();
		QueueElement<std::string> entry;

		if (!queue->tryPop(std::chrono::milliseconds(1000), entry))
			continue;
		
		if (!awaitOutput()) // wait again since pop() is blocking and could take a while
			return;
		
		printf(entry.element.c_str()); // print
		entry.result->set_value(true); // (idea) the promise could be used to know when the entry is effectively executed...
	}
}

bool OutputController::awaitOutput()
{
	std::unique_lock<std::mutex> lock(_waitForOutputLock);
	while (_consoleMode == ConsoleDevice::INPUT && _threadEnable)
	{
		_waitForOutput.wait_for(lock, std::chrono::milliseconds(100));
	}
	lock.unlock();

	return _threadEnable;
}
