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

#include <ghost/module/GhostLogger.hpp>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>
#include <ghost/persistence/SaveManager.hpp>

#include "protobuf/persistency_todo_list.pb.h"

/***************************
	TRY IT: Run this program and add or remove your own TODOs.
***************************/

class TodoListModule
{
public:
	// This method will initialize the ghost::SaveManager, which is used by this example
	// to save the TODO elements between program executions.
	// The structure of the save file used in this example is simple: it will contain a single
	// save file ("Todolist.dat"), which will contain a list of Google Protobuf messages defined
	// in the "examples/protobuf" folder of this repository (the example uses the "Todo" message).
	bool initialize(const ghost::Module& module)
	{
		// First, create an instance of the ghost::SaveManager, that uses "." as its root path to
		// save the data.
		_saveManager = ghost::SaveManager::create(".");
		std::string filename = TODO_LIST_NAME + ".dat";

		// Try to load existing data. It should return true if the program was already executed once.
		if (_saveManager->load({filename}))
		{
			// If data was loaded, we will work on the first data set (ghost::SaveData) available in the
			// save.
			auto existingData = _saveManager->getData(TODO_LIST_NAME);
			if (existingData.find(filename) != existingData.end() && existingData.at(filename).size() > 0)
			{
				_todoList = existingData.at(filename).front();
				GHOST_INFO(module.getLogger()) << "Loaded existing todo list.";
			}
		}
		if (!_todoList)
		{
			// If no data was loaded, let's create a first data set with the name "TodoList".
			// When "save" is called on the save manager, this data set will be saved in a file called
			// "TodoList.dat".
			_todoList = ghost::SaveData::create(TODO_LIST_NAME);
			_saveManager->addData(_todoList);
		}

		return true;
	}

	// The execution of the module reads the parameters provided to the program call.
	// It can list, add or remove parameters.
	bool run(const ghost::Module& module)
	{
		if (module.getProgramOptions().hasParameter("__0"))
		{
			if (module.getProgramOptions().getParameter<std::string>("__0") == "list")
				listTodo(module);
			else if (module.getProgramOptions().getParameter<std::string>("__0") == "add")
				addTodo(module);
			else if (module.getProgramOptions().getParameter<std::string>("__0") == "remove")
				removeTodo(module);
			else
				GHOST_INFO(module.getLogger()) << "Usage: program [list | add | remove]";
		}
		else
			GHOST_INFO(module.getLogger()) << "Usage: program [list | add | remove]";

		return false;
	}

	void listTodo(const ghost::Module& module)
	{
		if (_todoList->size() == 0)
			GHOST_INFO(module.getLogger()) << "There are no TODOs in the list";
		else
		{
			GHOST_INFO(module.getLogger()) << "Current TODOs: ";
			for (size_t i = 0; i < _todoList->size(); ++i)
			{
				// The ghost::SaveData stores Google Protobuf messages.
				// The following lines loads the content of the save file into a Todo Protobuf message.
				ghost::examples::protobuf::Todo todo;
				bool getResult = _todoList->get(todo, i);
				if (getResult) GHOST_INFO(module.getLogger()) << "TODO #" << i << ": " << todo.title();
			}
		}
	}

	void addTodo(const ghost::Module& module)
	{
		GHOST_INFO(module.getLogger()) << "Enter a title for the new TODO: ";
		auto title = module.getConsole()->getLine();
		auto todo = ghost::examples::protobuf::Todo::default_instance();
		todo.set_title(title);
		// To add a todo, this function simply requests a title from the user, creates a "Todo"
		// message and adds it to the ghost::SaveData object.
		_todoList->put(todo);
		// Once we are done working on the save, we can save it on the disk.
		_saveManager->save(true);
		GHOST_INFO(module.getLogger()) << "Added new TODO.";
	}

	void removeTodo(const ghost::Module& module)
	{
		GHOST_INFO(module.getLogger()) << "Enter the ID (#) of the TODO to remove: ";
		auto title = module.getConsole()->getLine();
		int id = std::stoi(title);
		// Similarly to the "addTodo" method, this method requests the ID of a TODO to remove
		// and removes it from the ghost::SaveData object.
		_todoList->remove(id);
		// Here again, we save the new content of the save to the disk.
		_saveManager->save(true);
		GHOST_INFO(module.getLogger()) << "Removed TODO";
	}

private:
	std::shared_ptr<ghost::SaveManager> _saveManager;
	std::shared_ptr<ghost::SaveData> _todoList;

	const std::string TODO_LIST_NAME = "TodoList";
};

int main(int argc, char** argv)
{
	TodoListModule myModule;

	// Configuration of the module. We provide here all the components to the builder.
	auto builder = ghost::ModuleBuilder::create();
	// This line will provide the intialization method.
	builder->setInitializeBehavior(std::bind(&TodoListModule::initialize, &myModule, std::placeholders::_1));
	// This line will provide the run method, which will be called cyclically.
	builder->setRunningBehavior(std::bind(&TodoListModule::run, &myModule, std::placeholders::_1));
	// Since we want to use the console logger, we provide one with a console to the builder.
	auto console = builder->setConsole();
	builder->setLogger(ghost::GhostLogger::create(console));
	// Parse the program options to determine what to do:
	builder->setProgramOptions(argc, argv);
	// The following line creates the module with all the parameters, and names it "myModuleInstance0".
	std::shared_ptr<ghost::Module> module = builder->build();

	// If the build process is successful, we can start the module. If it were not successful, we would have nullptr
	// here.
	if (module) module->start();

	// Start blocks until the module ends.
	return 0;
}
