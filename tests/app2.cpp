#include <iostream>
#include <sstream>

#include <internal/network/ClientGRPC.hpp>
#include <internal/network/SubscriberGRPC.hpp>
#include <ProtobufMessage.hpp>

using namespace ghost;

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

std::atomic_bool msgReceived;

void func(const internal::protobuf::GenericMessageHeader& msg)
{
	std::cout << "message received " << msg.timestamp() << std::endl;
	msgReceived = true;
}

class MyMessage : public Message
{
public:
	MyMessage() {}

	std::string getMessageFormatName() const override { return "TESTFORMAT"; }

	std::string getMessageTypeName() const override { return "MYMESSAGE"; }

	bool serialize(std::string& result) const override { return true; }

	bool deserialize(const std::string& payload) override { return true; }
};

void func2(const MyMessage& msg)
{
	std::cout << "special message received" << std::endl;
}

int main()
{
	msgReceived = false;
	NetworkConnectionConfiguration config;
	config.setServerIpAddress("127.0.0.1");
	config.setServerPortNumber(50001);

	internal::SubscriberGRPC<internal::protobuf::GenericMessageHeader> subscriber(config);
	subscriber.setMessageHandler(&func);
	subscriber.start();

	/*internal::ClientGRPC client(config);

	auto handler = client.addMessageHandler();
	handler->addHandler<internal::protobuf::GenericMessageHeader>(&func);
	handler->addHandler<MyMessage>(&func2);

	client.start();*/

	/*ProtobufMessage message = ProtobufMessage::create<internal::protobuf::GenericMessageHeader>();
	bool rec = client.receive(message);
	std::cout << "rec ?=" << rec << std::endl;

	auto msg = message.getProtobufMessage();
	auto casted = std::static_pointer_cast<internal::protobuf::GenericMessageHeader>(msg);

	std::cout << "server sent a message, happiness!!! : " << casted->timestamp() << std::endl;*/
	
	while (!msgReceived)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	subscriber.stop();
	//client.stop();
	return 0;
}
