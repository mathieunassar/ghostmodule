#ifndef GHOST_INTERNAL_NETWORK_BASECLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_BASECLIENTGRPC_HPP

#include "../../Client.hpp"

#include <string>
#include <deque>
#include <atomic>
#include <mutex>

#include "../GenericMessageConverter.hpp"
#include "../../ClientHandler.hpp"
#include "RPCStateMachine.hpp"

namespace ghost
{
	namespace internal
	{
		template<typename ReaderWriter, typename ContextType>
		class BaseClientGRPC : public ghost::Client
		{
		public:
			BaseClientGRPC(grpc::CompletionQueue* completionQueue);
			virtual ~BaseClientGRPC() = 0 {}

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

			bool receive(ghost::Message& message) override;
			bool lastReceived(ghost::Message& message) override;
			bool send(const ghost::Message& message) override;

		protected:
			void startReader();

			/* async operations management */
			std::atomic<int> _operationsRunning;
			void startOperation();
			bool finishOperation();
			void awaitFinished();

			/* Write operations */
			std::deque<google::protobuf::Any> _writeQueue;
			std::mutex _writeQueueMutex;
			std::atomic<bool> _writeOperationInProgress;
			std::function<void(bool)> _writtenProcessor;
			void processWriteQueue();
			void onWriteFinished(bool ok);

			/* Read operations */
			std::deque<google::protobuf::Any> _readQueue;
			std::mutex _readQueueMutex;
			google::protobuf::Any _incomingMessage;
			google::protobuf::Any _nextMessage;
			std::function<void(bool)> _readProcessor;
			void onReadFinished(bool ok);

			/* GRPC objects */
			RPCStateMachine _statemachine;
			grpc::CompletionQueue* _completionQueue;
			std::unique_ptr<ReaderWriter> _client;
			std::unique_ptr<ContextType> _context;
			void disposeGRPC();
		};

		#include "BaseClientGRPC.impl.hpp"
	}
}

#endif // GHOST_INTERNAL_NETWORK_BASECLIENTGRPC_HPP
