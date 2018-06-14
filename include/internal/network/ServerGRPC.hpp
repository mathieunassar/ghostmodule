#ifndef GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP
#define GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP

#include <grpcpp/server.h>
#include "../../../protobuf/Ghost.pb.h"
#include "../../../protobuf/Ghost.grpc.pb.h"
#include "../Server.hpp"

namespace ghost
{
	namespace internal
	{
		/**
		 * Server implementation using the gRPC library. Runs a gRPC server which accepts connections, and create
		 * a writing/sending interface which is returned to the server object.
		 * @author	Mathieu Nassar
		 * @date	25.05.2018
		 */
		class ServerGRPC : public Server, public protobuf::ServerClientService::Service
		{
		public:
			bool start() override;
			bool stop() override;
			bool isRunning() const override;

			void setNewClientCallback(std::function<bool(Client&)> callback) override;

		private:
			grpc::Status connect(grpc::ServerContext* context, grpc::ServerReaderWriter<google::protobuf::Any, google::protobuf::Any>* stream) override;

			std::unique_ptr<::grpc::Server> _grpcServer;
			std::function<bool(Client&)> _newClientCallback;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_SERVERGRPC_HPP
