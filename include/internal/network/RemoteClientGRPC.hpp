#ifndef GHOST_INTERNAL_NETWORK_REMOTECLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP

#include "../Client.hpp"

#include "../../../protobuf/Ghost.pb.h"
#include "../../../protobuf/Ghost.grpc.pb.h"

namespace ghost
{
	namespace internal
	{
		class RemoteClientGRPC : public Client
		{
		public:
			RemoteClientGRPC(grpc::ServerReaderWriter<google::protobuf::Any, google::protobuf::Any>* connection);

			bool start() override;
			bool stop() override;
			bool isRunning() const override;

			bool receive(ghost::Message& message) override;
			long send(const ghost::Message& message) override;

		private:
			grpc::ServerReaderWriter<google::protobuf::Any, google::protobuf::Any>* _connection;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
