#ifndef GHOST_INTERNAL_NETWORK_RPCSTATEMACHINE_HPP
#define GHOST_INTERNAL_NETWORK_RPCSTATEMACHINE_HPP

#include <mutex>
#include <atomic>

namespace ghost
{
	namespace internal
	{
		class RPCStateMachine
		{
		public:
			enum State
			{
				INIT,
				EXECUTING, INACTIVE,
				DISPOSING,
				FINISHED
			};

			RPCStateMachine();
			State getState(bool lock = true) const;
			void setState(State state, bool lock = true);

			void lock();
			void unlock();

		private:
			State _state;
			mutable std::mutex _mutex;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_RPCSTATEMACHINE_HPP
