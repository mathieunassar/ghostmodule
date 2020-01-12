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

#include <ghost/connection_extension/ConnectionExtension.hpp>
#include <ghost/connection_extension/ConnectionExtensionBuilder.hpp>
#include <ghost/connection_grpc/ConnectionConfigurationGRPC.hpp>
#include <ghost/connection_grpc/ConnectionGRPC.hpp>
#include <ghost/module/Command.hpp>
#include <ghost/module/GhostLogger.hpp>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>
#include <thread>

/***************************
	TRY IT:
	 The module presented in this example illustrates how to use the connection_extension library to create a
daemon/service and to communicate with it from a client. When the module is started as a service with the "daemon"
parameter, it sets up a server listening to incoming connections from clients that can remotely control it. When a
client is connected to a service, it forwards all the commands from its console to the service. In order to execute
commands on the client side, prefix the commands with "local:". Both daemon/client provide the "uptime" command. For
example: start the daemon and start a client. On the client's console, enter "uptime" and observe that the printed value
matches the uptime of the daemon. Then, enter "local:uptime": this time, the client executes the command locally and
prints its own uptime.
***************************/

/**
 *	The following command prints the uptime of this module.
 *	To invoke the command from the console, enter "uptime".
 */
class UptimeCommand : public ghost::Command
{
public:
	UptimeCommand()
	{
		_startTime = std::chrono::steady_clock::now();
	}

	// The execute method corresponds to the action of this command.
	bool execute(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context) override
	{
		// Use a ghost::GhostLogger to handle the console provided in the context.
		auto logger = ghost::GhostLogger::create(context.getConsole());
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -
										     _startTime);
		logger->info(std::to_string(elapsed.count()));
		return true;
	}

	std::string getName() const override
	{
		return "UptimeCommand";
	}
	std::string getShortcut() const override
	{
		return "uptime";
	}
	std::string getDescription() const override
	{
		return "Gets the number of milliseconds elapsed since the beginning of the program.";
	}

private:
	std::chrono::steady_clock::time_point _startTime;
};

/**
 *	Defines the execution of the module.
 *	The "initialize" method configures the remote access on the daemon and the remote control on the client
 *	and registers the "uptime" command.
 */
class ConnectionExtensionDaemonModule
{
public:
	// This method will be provided to the module builder as the "initialization method" of the program
	bool initialize(const ghost::Module& module)
	{
		ghost::ConnectionConfigurationGRPC config("127.0.0.1", 8001);

		// Providing "daemon" as first parameter starts the program as a service.
		if (module.getProgramOptions().hasParameter("__0") &&
		    module.getProgramOptions().getParameter<std::string>("__0") == "daemon")
		{
			GHOST_INFO(module.getLogger())
			    << "This is the service version of module " << module.getModuleName();
			// Sets "config" as the server configuration to listen to incoming remote control clients.
			module.getExtension<ghost::ConnectionExtension>()->addRemoteAccess(config);
		}
		else // Otherwise it is a client to the daemon.
		{
			GHOST_INFO(module.getLogger())
			    << "This is the remote controller version of " << module.getModuleName();
			// Sets "config" as the configuration of the server listening to incoming remote control
			// clients. Note: from that point, if the server is not available, the client program will fail
			// to start.
			module.getExtension<ghost::ConnectionExtension>()->setRemoteControl(config);
		}

		// The following two lines register an instance of the command defined previously.
		// The same instance will be invoked every time the user invokes the command!
		module.getInterpreter()->registerCommand(std::make_shared<UptimeCommand>());

		return true; // The initialization was successful, we can return true.
	}

	// This method will be called cyclically after we provide it to the module builder, until it returns false.
	bool run(const ghost::Module& module)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		return true; // we will loop forever. Users can invoke the built-in "exit" command to leave the program.
	}
};

int main(int argc, char** argv)
{
	ConnectionExtensionDaemonModule myModule;

	// Configuration of the module. We provide here all the components to the builder.
	auto builder = ghost::ModuleBuilder::create();
	// This line will provide the intialization method.
	builder->setInitializeBehavior(
	    std::bind(&ConnectionExtensionDaemonModule::initialize, &myModule, std::placeholders::_1));
	// This line will provide the run method, which will be called cyclically.
	builder->setRunningBehavior(std::bind(&ConnectionExtensionDaemonModule::run, &myModule, std::placeholders::_1));
	// We want to manipulate the console; the following line activates this feature.
	std::shared_ptr<ghost::Console> console = builder->setConsole();
	// The GhostLogger writes in the ghost::Console, which manages the inputs and outputs.
	builder->setLogger(ghost::GhostLogger::create(console));
	// Parse the program options to determine what to do:
	builder->setProgramOptions(argc, argv);

	auto connectedModuleBuilder = ghost::ConnectionExtensionBuilder::create();
	// Put GRPC definitions in the connection extension builder to allow it to create gRPC connections.
	ghost::ConnectionGRPC::initialize(connectedModuleBuilder->configureConnectionManager());
	// Add the component builder to the module builder
	builder->addExtensionBuilder(connectedModuleBuilder);

	// The following line creates the module with all the parameters, and names it "myModuleInstance0".
	std::shared_ptr<ghost::Module> module = builder->build("myModuleInstance0");
	// If the build process is successful, we can start the module.
	// If it were not successful, we would have nullptr here.
	if (module) module->start();

	// Start blocks until the module ends.
	return 0;
}
