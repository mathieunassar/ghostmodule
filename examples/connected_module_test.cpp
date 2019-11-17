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

#include <ghost/connected_module/ConnectedModule.hpp>
#include <ghost/connected_module/ConnectedModuleComponentBuilder.hpp>
#include <ghost/connection_grpc/ConnectionConfigurationGRPC.hpp>
#include <ghost/connection_grpc/ConnectionGRPC.hpp>
#include <ghost/module/Command.hpp>
#include <ghost/module/GhostLogger.hpp>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>
#include <thread>

/***************************
	TRY IT: Run this program. Press "enter" to access the user input mode.
	Type "updateText" and provide a new text to display. Enter the user input mode
	Again and type "exit" to quit the program.
***************************/

// In this example, the run method will print a message every second.
// We register in the initialization a command to let the user update the text of the message.
// While the module is running, the user can type the command and update the text.
class MyModule
{
public:
	// This method will be provided to the module builder as the "initialization method" of the program
	bool initialize(const ghost::Module& module)
	{
		// Among other things, the logger and the name of this module are reachable from these methods
		GHOST_INFO(module.getLogger()) << "This is module " << module.getModuleName();

		auto console = module.getConsole();
		console->write("Enter the IP address of the remote server to control: ");
		std::string ip = console->getLine();
		while (true)
		{
			try
			{
				console->write("Enter the port number of the remote server to control: ");
				std::string port = console->getLine();
				int portNumber = std::stoi(port);
				module.getComponent<ghost::ConnectedModule>()->setRemoteControl(
				    ghost::ConnectionConfigurationGRPC(ip, portNumber));
				break;
			}
			catch (const std::exception& e)
			{
				console->write("Invalid entry for the port number (it must be an integer).\n");
			}
		}

		return true; // The initialization was successful, we can return true.
	}

	// This method will be called cyclically after we provide it to the module builder, until it returns false.
	bool run(const ghost::Module& module)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		return true; // we will loop forever. Users can invoke the built-in "exit" command to leave the program.
	}
};

int main()
{
	MyModule myModule;

	// Configuration of the module. We provide here all the components to the builder.
	auto builder = ghost::ModuleBuilder::create();
	// This line will provide the intialization method.
	builder->setInitializeBehavior(std::bind(&MyModule::initialize, &myModule, std::placeholders::_1));
	// This line will provide the run method, which will be called cyclically.
	builder->setRunningBehavior(std::bind(&MyModule::run, &myModule, std::placeholders::_1));
	// We want to manipulate the console; the following line activates this feature.
	std::shared_ptr<ghost::Console> console = builder->setConsole();
	// The GhostLogger writes in the ghost::Console, which manages the inputs and outputs.
	builder->setLogger(ghost::GhostLogger::create(console));

	auto connectedModuleBuilder = ghost::ConnectedModuleComponentBuilder::create();
	// Put some GRPC definitions in the connection builder
	ghost::ConnectionGRPC::initialize(connectedModuleBuilder->configureConnectionManager());
	// Configure a remote access server on the localhost on port 8001
	ghost::ConnectionConfigurationGRPC config("127.0.0.1", 8001);
	//connectedModuleBuilder->addRemoteAccess(config);
	// Add the component builder to the module builder
	builder->addComponentBuilder(connectedModuleBuilder);

	// The following line creates the module with all the parameters, and names it "myModuleInstance0".
	std::shared_ptr<ghost::Module> module = builder->build("myModuleInstance0");
	// If the build process is successful, we can start the module. If it were not successful, we would have nullptr
	// here.
	if (module) module->start();

	// Start blocks until the module ends.
	return 0;
}
