#include "../../include/internal/network/ServerGRPC.hpp"

#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>

#include "../../include/internal/network/RemoteClientGRPC.hpp"
#include "../../include/internal/network/ServerGRPC.hpp"

using namespace ghost::internal;

ServerGRPC::ServerGRPC(const ghost::NetworkConnectionConfiguration& config)
	: _configuration(config)
	, _running(false)
{

}

ServerGRPC::~ServerGRPC()
{
	
}

bool ServerGRPC::start()
{
	if (_running)
		return false;

	_running = true;

	std::ostringstream server_address;
	server_address << _configuration.getServerIpAddress() << ":" << _configuration.getServerPortNumber();

	grpc::ServerBuilder builder;

	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address.str(), ::grpc::InsecureServerCredentials());

	// Register "_service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *asynchronous* service.
	builder.RegisterService(&_service);

	// Get hold of the completion queue used for the asynchronous communication
	// with the gRPC runtime.
	_completionQueueExecutor.setCompletionQueue(builder.AddCompletionQueue());

	// Finally assemble the server.
	_grpcServer = builder.BuildAndStart();

	_completionQueueExecutor.start(_configuration.getThreadPoolSize());

	auto cq = static_cast<grpc::ServerCompletionQueue*>(_completionQueueExecutor.getCompletionQueue());
	for (size_t i = 0; i < _configuration.getThreadPoolSize(); i++) // start as many calls as there can be concurrent rpcs
	{
		// Spawn a new CallData instance to serve new clients
		auto client = std::make_shared<RemoteClientGRPC>(_configuration, &_service, cq, _clientHandler, &_clientManager, this);
		_clientManager.addClient(client);
	}
	_clientManager.start();

	return true;
}

bool ServerGRPC::stop()
{
	if (!_running)
		return false;

	_running = false;

	_clientManager.stop();
	_grpcServer->Shutdown();
	_completionQueueExecutor.stop(); // shutdowns the completion queue

	return true;
}

bool ServerGRPC::isRunning() const
{
	return _running;
}

void ServerGRPC::setClientHandler(std::shared_ptr<ghost::ClientHandler> handler)
{
	_clientHandler = handler;
}
