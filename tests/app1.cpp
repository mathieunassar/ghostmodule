#include <iostream>
#include <sstream>

#include <internal/network/ServerGRPC.hpp>
#include <ProtobufMessage.hpp>

using namespace ghost;

class TestClientHandler : public ClientHandler
{
public:
	bool handle(Client& client, bool& keepClientAlive)
	{
		std::cout << "a cliant!" << std::endl;

		auto msg = std::make_shared<internal::protobuf::GenericMessageHeader>();
		msg->set_timestamp(500);

		long sentResult = client.send(ProtobufMessage(msg));
		std::cout << "sent a message to the lcient!! " << sentResult << std::endl;

		//keepClientAlive = true;

		static int john = 0;
		john++;

		return john < 2;
	}
};

int main()
{
	std::cout << "server starting" << std::endl;

	NetworkConnectionConfiguration config;
	config.setServerIpAddress("127.0.0.1");
	config.setServerPortNumber(50001);
	config.setThreadPoolSize(1);

	internal::ServerGRPC server(config);
	server.setClientHandler(std::make_shared<TestClientHandler>());
	server.start();
	std::cout << "server started" << std::endl;
	while (server.isRunning())
	{
		Sleep(100);
	}

	server.stop();
	std::cout << "exiting" << std::endl;
	return 0;
}
