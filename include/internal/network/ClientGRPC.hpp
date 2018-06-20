#ifndef GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
#define GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP

#include "../../Client.hpp"

#include "../../../protobuf/Ghost.pb.h"
#include "../../../protobuf/Ghost.grpc.pb.h"

namespace ghost
{
	namespace internal
	{
		class ClientGRPC : public ghost::Client
		{
		public:
			bool start() override;
			bool stop() override;
			bool isRunning() const override;

			bool receive(ghost::Message& message) override;
			long send(const ghost::Message& message) override;

		private:
			std::unique_ptr<grpc::ClientReaderWriter<google::protobuf::Any, google::protobuf::Any>> _connection;
			grpc::ClientContext _context;
		};
	}
}

#endif // GHOST_INTERNAL_NETWORK_CLIENTGRPC_HPP
