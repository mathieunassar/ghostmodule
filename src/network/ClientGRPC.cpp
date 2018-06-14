#include "../../include/internal/network/ClientGRPC.hpp"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "../../include/internal/GenericMessageConverter.hpp"

using namespace ghost::internal;

bool ClientGRPC::start()
{
	auto channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
	auto stub = protobuf::ServerClientService::NewStub(channel);

	_connection = stub->connect(&_context);

	return true;
}

bool ClientGRPC::stop()
{
	_connection->Finish();
	return true;
}

bool ClientGRPC::isRunning() const
{
	return true;
}

bool ClientGRPC::receive(ghost::Message& message)
{
	google::protobuf::Any any;
	bool readResult = _connection->Read(&any);
	if (!readResult)
	{
		return false;
	}

	// maybe store the message somewhere to get the last received message
	return GenericMessageConverter::parse(any, message);
}

long ClientGRPC::send(const ghost::Message& message)
{
	google::protobuf::Any any;

	bool createSuccess = GenericMessageConverter::create(any, message);
	if (!createSuccess)
	{
		return -1; // conversion failed, i.e. there is no protobuf message in the input or the serialization failed (in case of a user format)
	}

	return _connection->Write(any) ? 1 : -1; // TODO return value
}
