#include <iostream>
#include <sstream>

#include <internal/network/ServerGRPC.hpp>
#include <internal/network/PublisherGRPC.hpp>
#include <ConnectionManager.hpp>
#include <ProtobufMessage.hpp>

using namespace ghost;

class TestClientHandler : public ClientHandler
{
public:
	bool handle(std::shared_ptr<Client> client, bool& keepClientAlive)
	{
		auto writer = client->getWriter();
		std::cout << "a cliant!" << std::endl;

		auto msg = std::make_shared<internal::protobuf::GenericMessageHeader>();
		msg->set_timestamp(500);

		long sentResult = writer->write(ProtobufMessage(msg));
		std::cout << "sent a message to the lcient!! " << sentResult << std::endl;

		keepClientAlive = true;

		static int john = 0;
		john++;

		return john < 20;
	}
};

int main()
{
	std::cout << "server starting" << std::endl;

	NetworkConnectionConfiguration config;
	config.setServerIpAddress("127.0.0.1");
	config.setServerPortNumber(50001);
	config.setThreadPoolSize(8);

	auto connectionManager = ghost::ConnectionManager::create();
	auto factory = connectionManager->getConnectionFactory();
	factory->addPublisherRule<internal::PublisherGRPC>(config);

	auto publisher = connectionManager->createPublisher(config);

	auto writer = publisher->getWriter<internal::protobuf::GenericMessageHeader>();

	//auto reader = publisher->getReader<internal::protobuf::GenericMessageHeader>();

	publisher->start();

	size_t count = 0;
	while (count < 1000)
	{
		internal::protobuf::GenericMessageHeader msg;
		msg.set_timestamp(count);

		writer->write(msg);

		std::cout << "published message #" << count << ", server ok? " << publisher->isRunning() << std::endl;
		count++;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	/*internal::ServerGRPC server(config);
	server.setClientHandler(std::make_shared<TestClientHandler>());
	server.start();
	
	std::cout << "server started" << std::endl;
	while (server.isRunning())
	{
		Sleep(100);
	}*/

	std::cout << "exiting" << std::endl;
	publisher->stop();
	std::cout << "finished" << std::endl;
	return 0;
}
