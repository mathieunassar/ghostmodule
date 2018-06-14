#include <iostream>
#include <sstream>

#include <internal/network/ServerGRPC.hpp>
#include <ProtobufMessage.hpp>

using namespace ghost;

bool callback(internal::Client& client)
{
	std::cout << "a cliant!" << std::endl;

	auto msg = std::make_shared<internal::protobuf::GenericMessageHeader>();
	msg->set_timestamp(500);

	long sentResult = client.send(ProtobufMessage(msg));
	std::cout << "sent a message to the lcient!! " << sentResult << std::endl;

	return true;
}

int main()
{
	std::cout << "server starting" << std::endl;

	internal::ServerGRPC server;
	server.setNewClientCallback(&callback);
	server.start();
	std::cout << "servers started" << std::endl;
	Sleep(100000000);

	server.stop();
	std::cout << "exiting" << std::endl;
	return 0;
}
