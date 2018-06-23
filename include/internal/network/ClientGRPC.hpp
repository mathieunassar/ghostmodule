#ifndef GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP

#include <mutex>
#include <condition_variable>
#include "BaseClientGRPC.hpp"
#include "CompletionQueueExecutor.hpp"

namespace ghost
{
	namespace internal
	{
		class ClientGRPC : public BaseClientGRPC<grpc::ClientAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>, grpc::ClientContext>
		{
		public:
			ClientGRPC();

			bool start() override;
			bool stop() override;

		protected:
			void onStarted(bool ok);
			void onFinished(bool ok);

			std::function<void(bool)> _startedProcessor;
			std::function<void(bool)> _finishProcessor;
			std::unique_ptr<protobuf::ServerClientService::Stub> _stub;
			CompletionQueueExecutor _executor;

			std::mutex _initializedMutex;
			std::condition_variable _initializedConditionVariable;
			bool _initialized;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
