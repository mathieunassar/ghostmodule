#include "../../include/internal/network/RemoteClientGRPC.hpp"

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "../../include/internal/GenericMessageConverter.hpp"

using namespace ghost::internal;

RemoteClientGRPC::RemoteClientGRPC(grpc::ServerReaderWriter<google::protobuf::Any, google::protobuf::Any>* connection)
	: _connection(connection)
{

}

bool RemoteClientGRPC::start()
{
	return true;
}

bool RemoteClientGRPC::stop()
{
	return true;
}

bool RemoteClientGRPC::isRunning() const
{
	return true;
}

bool RemoteClientGRPC::receive(ghost::Message& message)
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

long RemoteClientGRPC::send(const ghost::Message& message)
{
	google::protobuf::Any any;

	bool createSuccess = GenericMessageConverter::create(any, message);
	if (!createSuccess)
	{
		return -1; // conversion failed, i.e. there is no protobuf message in the input or the serialization failed (in case of a user format)
	}

	return _connection->Write(any) ? 1 : -1; // TODO return value
}
