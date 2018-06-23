#include "../../include/internal/network/ServerGRPC.hpp"

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "../../include/internal/network/RemoteClientGRPC.hpp"
#include "../../include/internal/network/ServerGRPC.hpp"

using namespace ghost::internal;

ServerGRPC::ServerGRPC()
{

}

bool ServerGRPC::start()
{
	std::string server_address("127.0.0.1:50051");
	grpc::ServerBuilder builder;

	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, ::grpc::InsecureServerCredentials());

	// Register "_service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *asynchronous* service.
	builder.RegisterService(&_service);

	// Get hold of the completion queue used for the asynchronous communication
	// with the gRPC runtime.
	_completionQueueExecutor.setCompletionQueue(builder.AddCompletionQueue());

	// Finally assemble the server.
	_grpcServer = builder.BuildAndStart();

	_completionQueueExecutor.start(2);

	auto cq = static_cast<grpc::ServerCompletionQueue*>(_completionQueueExecutor.getCompletionQueue());
	for (size_t i = 0; i < 2; i++) // start as many calls as there can be concurrent rpcs
	{
		// Spawn a new CallData instance to serve new clients.
		new RemoteClientGRPC(&_service, cq, _clientHandler);
	}

	return true;
}

bool ServerGRPC::stop()
{
	_grpcServer->Shutdown();
	_completionQueueExecutor.stop(); // shutdowns the completion queue

	return true;
}

bool ServerGRPC::isRunning() const
{
	return true;
}

void ServerGRPC::setClientHandler(std::shared_ptr<ghost::ClientHandler> handler)
{
	_clientHandler = handler;
}
