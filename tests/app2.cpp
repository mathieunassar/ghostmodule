#include <iostream>
#include <sstream>

#include <internal/network/ClientGRPC.hpp>
#include <ProtobufMessage.hpp>

using namespace ghost;

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

int main()
{
	internal::ClientGRPC client;

	client.start();
	ProtobufMessage message = ProtobufMessage::create<internal::protobuf::GenericMessageHeader>();
	bool rec = client.receive(message);
	std::cout << "rec ?=" << rec << std::endl;

	auto msg = message.getProtobufMessage();
	auto casted = std::static_pointer_cast<internal::protobuf::GenericMessageHeader>(msg);

	std::cout << "server sent a message, happiness!!! : " << casted->timestamp() << std::endl;
	
	client.stop();
	return 0;
}
