/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ghost/connection/ClientHandler.hpp>
#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection_grpc/ConnectionGRPC.hpp>
#include <ghost/module/Command.hpp>
#include <ghost/module/GhostLogger.hpp>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>
#include <thread>

#include "protobuf/connection_grpc_messenger.pb.h"

/***************************
	TRY IT: Run this program parallely with the client and server configurations.
	Use the console feature to send text to the other side, and enter "#exit" to
	quit the program.
***************************/

// Handling method to process messages of type "ghost::examples::protobuf::MessengerMessage")
// This is provided to ghost::MessageHandler objects later in this example
void messengerMessageHandler(const ghost::examples::protobuf::MessengerMessage& message)
{
	std::cout << "[" << message.author() << "] " << message.content() << std::endl;
}

class MessengerModule;
// Implementation of a ghost::ClientHandler to store a client that is currently connected to the
// messenger, if this program is started.
// For readability, the implementation is located under the MessengerModule class in this file.
class MessengerClientHandler : public ghost::ClientHandler
{
public:
	MessengerClientHandler(MessengerModule* parent);
	void configureClient(const std::shared_ptr<ghost::Client>& client) override;
	bool handle(std::shared_ptr<ghost::Client> client, bool& keepClientAlive) override;

private:
	MessengerModule* _parent;
	std::shared_ptr<ghost::Client> _client;
};

/* Module's logic: MessengerModule */

// This class contains the logic of the messenger example.
// It contains an initialization method that will be called by the module once it is started, followed
// by the "run" method.
// The initialization reads command line parameters to know if the program is started as "server" or "client"
// of the messenger example.
// The run method simply waits and returns, as the functionality is handler by the console primarily.
class MessengerModule
{
public:
	// This method will be provided to the module builder as the "initialization method" of the program
	bool initialize(const ghost::Module& module)
	{
		if (module.getProgramOptions().hasParameter("__0"))
		{
			// Setup the connection manager and load gRPC connection implementations
			_connectionManager = ghost::ConnectionManager::create();
			ghost::ConnectionGRPC::initialize(_connectionManager);

			// Setup the configuration used by this example
			_configuration.setServerIpAddress("127.0.0.1");
			_configuration.setServerPortNumber(8562);

			// Set a custom callback for the console in order to process the messages entered by the user in
			// the console.
			module.getConsole()->setCommandCallback(std::bind(
			    &MessengerModule::consoleCallback, this, std::placeholders::_1, module.getInterpreter()));

			// Initialize the server or the client
			if (module.getProgramOptions().getParameter<std::string>("__0") == "server")
				return initializeServer(module.getLogger());
			else if (module.getProgramOptions().getParameter<std::string>("__0") == "client")
				return initializeClient(module.getLogger());
		}

		GHOST_INFO(module.getLogger()) << "Usage: program [client | server]";
		return false;
	}

	// Callback for commands entered in the console. If the line starts with '#', we forward the rest of the line
	// to the interpreter (which is almost the standard behavior). Otherwise, we write it to the connection that
	// registered a ghost::Writer in _writer.
	void consoleCallback(const std::string& cmd, const std::shared_ptr<ghost::CommandLineInterpreter>& interpreter)
	{
		if (cmd.length() > 0 && cmd[0] == '#')
			interpreter->execute(cmd.substr(1), ghost::CommandExecutionContext(ghost::Session::createLocal()));
		else if (_writer)
		{
			auto msg = ghost::examples::protobuf::MessengerMessage::default_instance();
			msg.set_author(_user);
			msg.set_content(cmd);

			if (_writer)
			{
				messengerMessageHandler(msg); // Write on the local console as well!
				bool writeResult = _writer->write(msg);
			}
		}
	}

	// Called by the MessengerClientHandler when a client connects to the messenger server.
	void registerWriter(const std::shared_ptr<ghost::Writer<ghost::examples::protobuf::MessengerMessage>>& writer)
	{
		_writer = writer;
	}

private:
	bool initializeClient(const std::shared_ptr<ghost::Logger>& logger)
	{
		GHOST_INFO(logger) << "Configured as client";
		_user = "client";
		// "_configuration" is of type "ghost::ConnectionConfigurationGRPC", which makes sure that the
		// configuration has the required attributes to be recognized by the connection factory operated by the
		// connect manager.
		auto client = _connectionManager->createClient(_configuration);
		// Adds a message handler to the ReaderSink of the client connection. After doing this, a ghost::Reader
		// will not be able to get message since they will be forwarded to the message handler.
		auto messageHandler = client->addMessageHandler();
		// Sets the handler of "ghost::examples::protobuf::MessengerMessage" messages (it simply prints the
		// content)
		messageHandler->addHandler<ghost::examples::protobuf::MessengerMessage>(&messengerMessageHandler);
		// Register a writer from the client connection to be able to send messages to the server.
		_writer = client->getWriter<ghost::examples::protobuf::MessengerMessage>();

		// The client is now owned by the connection manager, no need to store it anywhere.
		return client->start();
	}

	bool initializeServer(const std::shared_ptr<ghost::Logger>& logger)
	{
		GHOST_INFO(logger) << "Configured as server";
		_user = "server";
		// Similarly to the client's initialization, _configuration contains the minimum set of parameters
		// required by gRPC connection implementations.
		auto server = _connectionManager->createServer(_configuration);
		// Provides an instance of the "MessengerClientHandler" to the server. It will call its methods when new
		// clients connect to the server.
		server->setClientHandler(std::make_shared<MessengerClientHandler>(this));

		// The server is now owned by the connection manager, no need to store it anywhere.
		return server->start();
	}

	// On destruction, the connection manager will stop all previously created connections.
	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
	ghost::ConnectionConfigurationGRPC _configuration;
	std::shared_ptr<ghost::Writer<ghost::examples::protobuf::MessengerMessage>> _writer;
	std::string _user;
};

/* Implementation of the client handler */

MessengerClientHandler::MessengerClientHandler(MessengerModule* parent) : _parent(parent)
{
}

// This method will be called when a new client connects to the server.
// When this method is called, the read and write operations are not started yet,
// which allows the user to configure a ghost::MessageHandler or to perform other operations before the
// client is handled.
void MessengerClientHandler::configureClient(const std::shared_ptr<ghost::Client>& client)
{
	auto messageHandler = client->addMessageHandler();
	messageHandler->addHandler<ghost::examples::protobuf::MessengerMessage>(&messengerMessageHandler);
}

// After the client connected and "configureClient" was called, this method is called. It should
// contain the operations that the server needs to perform with the client (e.g. read, write...)
// The parameter "keepClientAlive" allows users to store the "client" in case the communication must be
// kept alive.
// The return value of this method will determine whether the server continues running after processing
// the client.
bool MessengerClientHandler::handle(std::shared_ptr<ghost::Client> client, bool& keepClientAlive)
{
	std::cout << "New client is connected!" << std::endl;
	if (_client)
	{
		std::cout << "Another client was running, closing it and replacing it." << std::endl;
		_client->stop();
	}
	_client = client;
	_parent->registerWriter(_client->getWriter<ghost::examples::protobuf::MessengerMessage>());

	// the client is stored in _client, we don't want the server to shutdown the connection after this call.
	// The server still owns the client, and will stop it when it gets stopped.
	keepClientAlive = true;
	return true;
}

/* main function: instantiation of the ghost::Module */

int main(int argc, char** argv)
{
	MessengerModule myModule;

	// Configuration of the module. We provide here all the components to the builder.
	auto builder = ghost::ModuleBuilder::create();
	// This line will provide the intialization method.
	builder->setInitializeBehavior(std::bind(&MessengerModule::initialize, &myModule, std::placeholders::_1));
	// The module will run until the user enters the "#exit" command in the console, hence we return "true" after
	// waiting for a little bit.
	builder->setRunningBehavior([](const ghost::Module&) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		return true;
	});
	// Since we want to use the console logger, we provide one with a console to the builder.
	auto console = builder->setConsole();
	builder->setLogger(ghost::GhostLogger::create(console));
	// Parse the program options to determine what to do:
	builder->setProgramOptions(argc, argv);

	// The following line creates the module with all the parameters, and names it "ghostMessengerExample".
	std::shared_ptr<ghost::Module> module = builder->build("ghostMessengerExample");
	// If the build process is successful, we can start the module. If it were not successful, we would have nullptr
	// here.
	if (module) module->start();

	// Start blocks until the module ends.
	return 0;
}
