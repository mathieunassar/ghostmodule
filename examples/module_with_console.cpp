/*
 * Copyright 2020 Mathieu Nassar
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
	// This is our command - it inherits from an abstract command, which defines the methods
	// that will be called by the interpreter.
	class UpdateTextCommand : public ghost::Command
	{
	public:
		UpdateTextCommand(MyModule* parent, std::shared_ptr<ghost::Console> console)
		    : _parent(parent), _console(console)
		{
		}

		// The execute method corresponds to the action of this command.
		bool execute(const ghost::CommandLine& commandLine,
			     const ghost::CommandExecutionContext& context) override
		{
			// A password can be queried from the user: their input will not be displayed
			std::cout << "Enter password: (hint: it's \"ghost\") " << std::flush;
			std::string hiddenLine = _console->getLine(true);
			std::cout << "Your password is: " << hiddenLine << "..." << std::endl;
			// When we activated the console, we also redirected the stdout stream to it.
			std::cout << "Enter new text: " << std::flush;
			// Gets a line and blocks the output stream while the user writes something
			std::string line = _console->getLine();
			// Call MyModule with the new text!
			_parent->updateText(line);
			return true;
		}

		std::string getName() const override
		{
			return "UpdateTextCommand";
		}
		// This method defines the command that he user will have to enter to invoke this command
		std::string getShortcut() const override
		{
			return "updateText";
		}
		std::string getDescription() const override
		{
			return "Updates the text of MyModule.";
		}

	private:
		MyModule* _parent;
		std::shared_ptr<ghost::Console> _console;
	};

	// This method will be provided to the module builder as the "initialization method" of the program
	bool initialize(const ghost::Module& module)
	{
		// Among other things, the logger and the name of this module are reachable from these methods
		GHOST_INFO(module.getLogger()) << "This is module " << module.getModuleName();

		_theText = "some text";

		// The following two lines register an instance of the command defined previously.
		// The same instance will be invoked every time the user invokes the command!
		auto command = std::make_shared<UpdateTextCommand>(this, module.getConsole());
		module.getInterpreter()->registerCommand(command);

		return true; // The initialization was successful, we can return true.
	}

	// This method will be called cyclically after we provide it to the module builder, until it returns false.
	bool run(const ghost::Module& module)
	{
		GHOST_INFO(module.getLogger()) << _theText;
		// We could as well use the next line - if the output is blocked by ghost::Console, std::cout is also
		// blocked
		// std::cout << _theText << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));

		return true; // we will loop forever. Users can invoke the built-in "exit" command to leave the program.
	}

	void dispose(const ghost::Module& module)
	{
		GHOST_INFO(module.getLogger()) << "disposed";
	}

	void updateText(const std::string& text)
	{
		// note: _theText is accessed concurrently by the command and by MyModule::run.
		// But for the sake of simplicity for this example, we don't use a mutex here.
		_theText = text;
	}

private:
	std::string _theText;
};

int main()
{
	MyModule myModule;

	// Configuration of the module. We provide here all the components to the builder.
	auto builder = ghost::ModuleBuilder::create();
	builder->getThreadPool()->resize(0);
	// This line will provide the intialization method.
	builder->setInitializeBehavior(std::bind(&MyModule::initialize, &myModule, std::placeholders::_1));
	// This line will provide the run method, which will be called cyclically.
	builder->setRunningBehavior(std::bind(&MyModule::run, &myModule, std::placeholders::_1));
	// This line will provide the dispose method, which will be called when the module disposes.
	builder->setDisposeBehavior(std::bind(&MyModule::dispose, &myModule, std::placeholders::_1));
	// We want to manipulate the console; the following line activates this feature.
	std::shared_ptr<ghost::Console> console = builder->setConsole();
	// The GhostLogger writes in the ghost::Console, which manages the inputs and outputs.
	builder->setLogger(ghost::GhostLogger::create(console));

	// The following line creates the module with all the parameters, and names it "myModuleInstance0".
	std::shared_ptr<ghost::Module> module = builder->build("myModuleInstance0");
	// If the build process is successful, we can start the module. If it were not successful, we would have nullptr
	// here.
	if (module) module->start();

	// Start blocks until the module ends.
	return 0;
}
