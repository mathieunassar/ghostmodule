#ifndef GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "BaseClientGRPC.hpp"
#include "../../ClientHandler.hpp"

namespace ghost
{
	namespace internal
	{
		class RemoteClientGRPC : public BaseClientGRPC<grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>, grpc::ServerContext>
		{
		public:
			RemoteClientGRPC(const NetworkConnectionConfiguration& config,
				protobuf::ServerClientService::AsyncService* service, grpc::ServerCompletionQueue* completionQueue,
				std::shared_ptr<ClientHandler> callback);

			bool start() override;
			bool stop() override;

		protected:
			void onStarted(bool ok);
			void onFinished(bool ok);
			void onDone(bool ok);
			void execute();

			std::function<void(bool)> _startedProcessor;
			std::function<void(bool)> _finishProcessor;
			std::function<void(bool)> _doneProcessor;
			protobuf::ServerClientService::AsyncService* _service;
			std::shared_ptr<ClientHandler> _clientHandler;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
