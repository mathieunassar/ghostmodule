#include "../../include/internal/network/ServerGRPC.hpp"

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "../../include/internal/network/RemoteClientGRPC.hpp"

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
	_completionQueue = builder.AddCompletionQueue();

	// Finally assemble the server.
	_grpcServer = builder.BuildAndStart();

	handleRpcs(); // here call this into many threads

	return true;
}

bool ServerGRPC::stop()
{
	_grpcServer->Shutdown();
	_completionQueue->Shutdown();
	return true;
}

bool ServerGRPC::isRunning() const
{
	return true;
}

void ServerGRPC::setClientHandler(std::shared_ptr<ClientHandler> handler)
{
	_clientHandler = handler;
}

void ServerGRPC::handleRpcs()
{
	// Spawn a new CallData instance to serve new clients.
	new RemoteClientGRPC(&_service, _completionQueue.get(), _clientHandler);

	TagInfo tag;
	while (true)
	{
		// Block waiting to read the next event from the completion queue. The
		// event is uniquely identified by its tag, which in this case is the
		// memory address of a CallData instance.
		// The return value of Next should always be checked. This return value
		// tells us whether there is any kind of event or cq_ is shutting down.
		bool nextSuccess = _completionQueue->Next((void**)&tag.processor, &tag.ok);
		if (!nextSuccess || !tag.ok)
		{
			std::cout << "an error occurred!" << std::endl;
			break;
		}

		(*tag.processor)(tag.ok);
	}
}
