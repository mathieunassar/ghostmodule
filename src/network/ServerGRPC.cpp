#include "../../include/internal/network/ServerGRPC.hpp"

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "../../include/internal/network/RemoteClientGRPC.hpp"

using namespace ghost::internal;

bool ServerGRPC::start()
{
	std::string server_address("127.0.0.1:50051");
	::grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, ::grpc::InsecureServerCredentials());
	builder.RegisterService(this);
	_grpcServer = builder.BuildAndStart();
	std::cout << "Server listening on " << server_address << std::endl;

	_grpcServer->Wait();

	return true;
}

bool ServerGRPC::stop()
{
	_grpcServer->Shutdown();
	_grpcServer->Wait(); // useful?
	return true;
}

bool ServerGRPC::isRunning() const
{
	return true;
}

void ServerGRPC::setNewClientCallback(std::function<bool(Client&)> callback)
{
	_newClientCallback = callback;
}

::grpc::Status ServerGRPC::connect(::grpc::ServerContext* context, ::grpc::ServerReaderWriter<google::protobuf::Any, google::protobuf::Any>* stream)
{
	std::cout << "hello" << std::endl;
	RemoteClientGRPC client(stream);

	if (_newClientCallback)
	{
		_newClientCallback(client);
	}

	return ::grpc::Status::OK;
}
