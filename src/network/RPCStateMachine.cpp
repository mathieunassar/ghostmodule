#include "../../include/internal/network/RPCStateMachine.hpp"

using namespace ghost::internal;

RPCStateMachine::RPCStateMachine()
	: _state(INIT)
{

}

RPCStateMachine::State RPCStateMachine::getState(bool lockk) const
{
	if (lockk)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _state;
	}

	return _state;
}

void RPCStateMachine::setState(State state, bool lockk)
{
	if (lockk)
		lock();

	bool transitionAllowed = false;
	switch (_state)
	{
	case INIT:
		transitionAllowed = true;
		break;
	case EXECUTING:
	case INACTIVE:
		transitionAllowed = (state == DISPOSING || state == FINISHED);
		break;
	case DISPOSING:
		transitionAllowed = (state == FINISHED);
		break;
	case FINISHED:
		transitionAllowed = false;
		break;
	default: break;
	}

	if (transitionAllowed)
		_state = state;

	if (lockk)
		unlock();
}

void RPCStateMachine::lock()
{
	_mutex.lock();
}

void RPCStateMachine::unlock()
{
	_mutex.unlock();
}
