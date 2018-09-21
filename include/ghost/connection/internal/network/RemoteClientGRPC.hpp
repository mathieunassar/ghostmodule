#ifndef GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP

#include <thread>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include <ghost/connection/internal/network/BaseClientGRPC.hpp>
#include <ghost/connection/internal/network/ClientManager.hpp>
#include <ghost/connection/Server.hpp>
#include <ghost/connection/ClientHandler.hpp>

namespace ghost
{
	namespace internal
	{
		class RemoteClientGRPC : public BaseClientGRPC<grpc::ServerAsyncReaderWriter<google::protobuf::Any, google::protobuf::Any>, grpc::ServerContext>
		{
		public:
			RemoteClientGRPC(const NetworkConnectionConfiguration& config,
				protobuf::ServerClientService::AsyncService* service, grpc::ServerCompletionQueue* completionQueue,
				std::shared_ptr<ClientHandler> callback,
				ClientManager* clientManager,
				ghost::Server* server);
			~RemoteClientGRPC();

			bool start() override;
			bool stop() override;

			void dispose();

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
			std::thread _executionThread;

			ClientManager* _clientManager;
			ghost::Server* _server;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
