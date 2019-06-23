``` ascii
 _______ _      _  ______  ________ _________
|  _____ |______| |      | |_______     |
|______| |      | |______| _______|     |
```

# ghostmodule

Lightweight, multiplatform and accessible framework to create command line-based programs.

|    Build system    |                         Build status                         |
| :----------------: | :----------------------------------------------------------: |
|    Windows x64     | [![Build status](https://ci.appveyor.com/api/projects/status/urayu2uebhqq3m6y?svg=true)](https://ci.appveyor.com/project/mathieunassar/ghostmodule) |
| Linux x64 (xenial) | [![Build Status](https://travis-ci.com/mathieunassar/ghostmodule.svg?branch=master)](https://travis-ci.com/mathieunassar/ghostmodule) |

## Overview

ghostmodule is a C++ library providing a framework for simple command line applications. It provides the following functionalities:

- **Program lifetime management**: separates the different stages of the program's life: its initialization, runtime and finalization;
- **Console control**: provides a way to input commands, and controls the output flow to pause when the User is typing;
- **Command interpretation**: optionally processes user input as commands, previously defined by the developer;
- **User management**: exposes a login system to restrict the access to some commands and program features;
- **Data persistence**: provides a sub-library based on Google's Protobuf to store data into save files;
- **Multiplatform**: Linux (Ubuntu Xenial, GCC compilers) and Windows (MSVC compilers) platforms are currently supported.

This README file contains the installation instructions, as well as a brief introduction to the features provided by ghostmodule.

## Setup

Being lightweight, the installation of ghostmodule is simple and straightforward.

ghostmodule uses CMake (<https://cmake.org/>) and Conan (<https://conan.io/>) for the dependency management.

The following external dependencies are used by this project:

- **Googletest** (<https://github.com/google/googletest>): used for the unit testing;
- **Google Protobuf** (<https://github.com/protocolbuffers/protobuf>): used to serialize and store data into storage files.

It is possible to build (parts of) the project without any of these dependencies.

When building requires one of these dependencies, **Conan** must be installed and "`conan`" must be callable from a bash console. It can be easily installed with Python3's pip install tool.

#### Simple installation (without unit tests, with examples)

Per default, building the unit tests and the examples is disabled. To build the library, simply execute the CMake script, as in the following example for Windows:

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 15 Win64" -DBUILD_EXAMPLES=True
cmake --build . --config Release
```

For debug builds, with the Linux version:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_EXAMPLES=True
cmake --build .
```

If "`BUILD_EXAMPLES=True`" is not specified, the examples will not be built.

*Note: during the configure phase of CMake, a support repository is downloaded from Github, which contains implementation details: ghostsupport.*

#### Full installation (with the unit tests)

To build the unit tests and the example programs, the following script is used:

```bash
mkdir build
cd build
cmake .. -DBUILD_TESTS=True -DBUILD_EXAMPLES=True -G "Visual Studio 15 Win64"
cmake --build .
ctest .
```

*Note: multi-configurations generators is not supported by this project. In order to switch from a Debug build to a Release build (or the contrary), `cmake` must be executed again with the corresponding value for the parameter "`CMAKE_BUILD_TYPE`".*

#### Partial installation

The following CMake variables can be used to perform partial builds:

- **BUILD_TESTS**: if set to "True", the unit tests will be built (GTest will be fetched by Conan).
  The default value of this parameter is "False".
- **BUILD_EXAMPLES**: if set to "True", example programs will be built.
  The default value of this parameter is "False".
- **BUILD_MODULE**: if set to "True", the library "ghost_module" will be built.
  The default value of this parameter is "True".
- **BUILD_PERSISTENCE**: if set to "True", the library "ghost_persistence" will be built.
  The default value of this parameter is "True".

For example, you can replace the CMake calls above by the following line to build everything:

```cmake
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=True -DBUILD_PERSISTENCE=True -DBUILD_MODULE=True -DBUILD_EXAMPLES=True -G "Visual Studio 15 Win64"
```

#### Use ghostmodule as a submodule

ghostmodule builds quickly and can be integrated in your project by adding the repository as a submodule.

To use the project using this configuration, simply initialize your submodule as follows:

```bash
git submodule update --init --recursive
```

Then, add the submodule in your CMake script, add the headers to your include directories, and link your targets against the library:

```cmake
add_subdirectory(ghostmodule)
...
# The variable GHOST_MODULE_INCLUDE_DIRECTORIES
# is generated by ghostmodule's CMakeLists
include_directories(${GHOST_MODULE_INCLUDE_DIRECTORIES})
...
# The targets "ghost_module" and "ghost_persistence"
# are created by ghostmodule's CMakeLists
target_link_libraries(yourTarget ghost_module)
target_link_libraries(yourTarget ghost_persitence)
```

## Getting Started

Three examples are provided to grasp the basic features provided by ghostmodule:

- examples/module_minimum.cpp: The program shows how to create a simple module and illustrates the feature "**program lifetime management**", which is contained in the library "ghost_module".
- examples/module_with_console.cpp: This example implements a program that cyclically displays a text, and that provides a command to update the text. The program uses the "**console control**" feature to request a user input, and the "**command interpretation**" feature to automatically process the command. These features are also contained in the library "ghost_module".
- examples/persistence_todo_list.cpp: In this example, a TODO list is managed by successive calls to the program with different parameters. By using the "**data persistence**" feature, the list is stored in a file and loaded when the program is used to edit it.

In order to build the examples, pass the flag "`-DBUILD_EXAMPLES=True`" to CMake. After quickly inspecting the source code, you can execute the programs and play around with them.

To continue working with this library, please read the documentation contained in the API header files contained in the folders include/ghost/module and include/ghost/persistence.

## Feedback

If you are using this library, find a bug, have a recommendation or anything else, I would be glad to hear from you!