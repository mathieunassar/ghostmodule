#ifndef GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP

#include "../../Client.hpp"

#include <string>
#include <deque>
#include <atomic>
#include <mutex>
#include "../../../protobuf/Ghost.pb.h"
#include "../../../protobuf/Ghost.grpc.pb.h"

#include "ServerGRPC.hpp"
#include "../../ClientHandler.hpp"

namespace ghost
{
	namespace internal
	{
		class RemoteClientGRPC : public ghost::Client
		{
		public:
			RemoteClientGRPC(protobuf::ServerClientService::AsyncService* service, grpc::ServerCompletionQueue* completionQueue,
				std::shared_ptr<ClientHandler> callback);

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

			bool receive(ghost::Message& message) override;
			bool lastReceived(ghost::Message& message) override;
			bool send(const ghost::Message& message) override;

		private:
			/* Completion queue callbacks */
			void onStarted(bool ok);
			void onFinished(bool ok);
			void onWriteFinished(bool ok);
			void onReadFinished(bool ok);
			void onDone(bool ok);
			std::function<void(bool)> _startedProcessor;
			std::function<void(bool)> _finishProcessor;
			std::function<void(bool)> _writtenProcessor;
			std::function<void(bool)> _readProcessor;
			std::function<void(bool)> _doneProcessor;
			std::atomic<int> _operationsRunning;
			void startOperation();
			bool finishOperation();

			/* Write operations */
			std::deque<google::protobuf::Any> _writeQueue;
			std::mutex _writeQueueMutex;
			std::atomic<bool> _writeOperationInProgress;
			void processWriteQueue();

			/* Read operations */
			std::deque<google::protobuf::Any> _readQueue;
			std::mutex _readQueueMutex;
			google::protobuf::Any _incomingMessage;
			google::protobuf::Any _nextMessage;
			
			/* Lifetime methods */
			void execute();

			/* GRPC objects */
			protobuf::ServerClientService::AsyncService* _service;
			grpc::ServerCompletionQueue* _completionQueue;
			std::shared_ptr<ClientHandler> _clientHandler;
			/// grpc context (can be used to add authentication, compression...)
			grpc::ServerContext _context;
			/// Connection to the client
			grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any> _client;

			/* RPC status */
			enum CallStatus
			{
				INIT,
				EXECUTING,
				DISPOSING,
				FINISHED
			};
			CallStatus _status;
			mutable std::mutex _statusMutex;
			CallStatus getStatus() const;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
