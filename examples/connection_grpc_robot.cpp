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

#include <ghost/connection/ConnectionManager.hpp>
#include <ghost/connection_grpc/ConnectionGRPC.hpp>
#include <ghost/module/Command.hpp>
#include <ghost/module/GhostLogger.hpp>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>
#include <thread>

#include "protobuf/connection_grpc_robot.pb.h"

/***************************
	TRY IT: Run this program once with the program option "robot". Using the ghost::Console,
	enter "updateVel 1.0 0.0" to change the current velocity of the robot to 1.0 m/s.
	The program automatically subscribes to the robot and displays the current odometry.
	You can start the program multiple times without any program options to start new subscribers
	to the odometry.
***************************/

// Handling method to process messages of type "ghost::examples::protobuf::RobotOdometry")
// This is provided to ghost::MessageHandler objects later in this example
void odmetryMessageHandler(const ghost::examples::protobuf::RobotOdometry& message)
{
	std::cout << "Received odometry: " << message.x() << "; " << message.y() << " [m/s; m/s]" << std::endl;
}

// The following class represents the robot of this example. The robot is very simple, it only has
// an (x,y) odometry and drives with an (x,y) velocity, which updates the velocity.
// The robot possesses a ghost::Publisher through which it publishes its current odometry when "update"
// is called.
// "setVelocity" is called by a ghost::Command, registered to the ghost::CommandLineInterpreter later in
// this program.
class Robot
{
public:
	Robot(const std::shared_ptr<ghost::ConnectionManager>& connectionManager,
	      const ghost::ConnectionConfigurationGRPC& config)
	    : _odoX(0.0), _odoY(0.0), _velX(0.0), _velY(0.0), _lastTime(std::chrono::steady_clock::now())
	{
		// Creates a publisher with the provided configuration. The publisher belongs to the connection manager
		// and therefore doesn't have to be stored or deleted.
		// Once the publisher is started, we keep a reference to a writer of "RobotOdometry" messages.
		auto publisher = connectionManager->createPublisher(config);
		if (publisher->start())
			_odometryWriter = publisher->getWriter<ghost::examples::protobuf::RobotOdometry>();
	}

	void setVelocity(double x, double y)
	{
		_velX = x;
		_velY = y;
		std::cout << "Updated velocity to " << _velX << "; " << _velY << " [m/s; m/s]" << std::endl;
	}

	void update()
	{
		if (!_odometryWriter) return;

		auto delta = std::chrono::steady_clock::now() - _lastTime;
		// Perform very complex operations to compute the super-high-resolution odometry.
		_odoX = _odoX + _velX * std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() / 1000;
		_odoY = _odoY + _velY * std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() / 1000;
		_lastTime += delta;

		// Send the message through the publisher with the help of the writer
		auto msg = ghost::examples::protobuf::RobotOdometry::default_instance();
		msg.set_x(_odoX);
		msg.set_y(_odoY);
		// "write" also returns the result of the operation, which could fail if the connection was stopped.
		_odometryWriter->write(msg);
	}

private:
	std::shared_ptr<ghost::Writer<ghost::examples::protobuf::RobotOdometry>> _odometryWriter;
	double _odoX, _odoY;
	double _velX, _velY;
	std::chrono::steady_clock::time_point _lastTime;
};

// This is the ghost::Command that gets triggered when the user writes "updateVel" in the ghost::Console
// It parses the new velocity (x and y) and passes it to the "Robot" instance.
// Note that this example only registers the command if the "robot" program option is passed.
// Currently it is impossible to pass negative values as parameters as they will be parsed into new parameters!!
class UpdateVelocityCommand : public ghost::Command
{
public:
	UpdateVelocityCommand(const std::shared_ptr<Robot>& robot) : _robot(robot)
	{
	}

	// The execute method corresponds to the action of this command.
	bool execute(const ghost::CommandLine& commandLine, const ghost::CommandExecutionContext& context) override
	{
		if (!commandLine.hasParameter("__0") || !commandLine.hasParameter("__1") || !_robot) return false;

		double vx = commandLine.getParameter<double>("__0");
		double vy = commandLine.getParameter<double>("__1");
		_robot->setVelocity(vx, vy);

		return true;
	}

	std::string getName() const override
	{
		return "UpdateVelocityCommand";
	}
	// This method defines the command that he user will have to enter to invoke this command
	std::string getShortcut() const override
	{
		return "updateVel";
	}
	std::string getDescription() const override
	{
		return "Updates the robot's velocity";
	}

private:
	std::shared_ptr<Robot> _robot;
};

/* Module's logic: RobotModule */

// This class contains the logic of the robot example.
// It contains an initialization method that will be called by the module once it is started, followed
// by the "run" method.
// The initialization creates the connection manager, sets a configuration, optionally creates a "Robot"
// object as defined above, and subscribes to the publisher of an existing "Robot" instance.
// Obviously, the program must be running at least once with the "robot" option to function.
// The run method updates the robot if it is configured.
class RobotModule
{
public:
	// This method will be provided to the module builder as the "initialization method" of the program
	bool initialize(const ghost::Module& module)
	{
		// Setup the connection manager and load gRPC connection implementations
		_connectionManager = ghost::ConnectionManager::create();
		ghost::ConnectionGRPC::initialize(_connectionManager);

		// Setup the configuration used by this example
		_configuration.setServerIpAddress("127.0.0.1");
		_configuration.setServerPortNumber(8562);

		if (module.getProgramOptions().hasParameter("__0") &&
		    module.getProgramOptions().getParameter<std::string>("__0") == "robot")
		{
			_robot = std::make_shared<Robot>(_connectionManager, _configuration);

			// The following two lines register an instance of the command defined previously.
			// The same instance will be invoked every time the user invokes the command!
			auto command = std::make_shared<UpdateVelocityCommand>(_robot);
			module.getInterpreter()->registerCommand(command);
		}

		// Creates the subscriber to the existing robot publisher.
		// Similarly to the publisher, the subscriber will belong to the connection manager and doesn't have
		// to be managed by this class.
		auto subscriber = _connectionManager->createSubscriber(_configuration);
		// Here as well, the message handler belongs to the subscriber and doesn't have to be managed.
		auto messageHandler = subscriber->addMessageHandler();
		messageHandler->addHandler<ghost::examples::protobuf::RobotOdometry>(&odmetryMessageHandler);

		// The intialization will fail if the subscriber cannot connect to a publisher,
		// i.e. if this program was not run with the "robot" option (exactly) once!
		return subscriber->start();
	}

	bool run(const ghost::Module& module)
	{
		if (_robot) _robot->update();
		// The robot will send new odometry data (roughly) with a 20 Hz frequency
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return true;
	}

private:
	// On destruction, the connection manager will stop all previously created connections.
	std::shared_ptr<ghost::ConnectionManager> _connectionManager;
	ghost::ConnectionConfigurationGRPC _configuration;
	std::shared_ptr<Robot> _robot;
};

/* main function: instantiation of the ghost::Module */

int main(int argc, char** argv)
{
	RobotModule myModule;

	// Configuration of the module. We provide here all the components to the builder.
	auto builder = ghost::ModuleBuilder::create();
	// This line will provide the intialization method.
	builder->setInitializeBehavior(std::bind(&RobotModule::initialize, &myModule, std::placeholders::_1));
	// The module will run until the user enters the "#exit" command in the console, hence we return "true" after
	// waiting for a little bit.
	builder->setRunningBehavior(std::bind(&RobotModule::run, &myModule, std::placeholders::_1));
	// We will use a ghost::Console in this example to control the inputs while the odometry is being printed
	(void)builder->setConsole();
	// Parse the program options to determine what to do:
	builder->setProgramOptions(argc, argv);

	// The following line creates the module with all the parameters, and names it "ghostRobotExample".
	std::shared_ptr<ghost::Module> module = builder->build("ghostRobotExample");
	// If the build process is successful, we can start the module. If it were not successful, we would have nullptr
	// here.
	if (module) module->start();

	// Start blocks until the module ends.
	return 0;
}
