``` ascii
                                                   ____
_______  _      _  ______  ________ _________      '_' \
|  _____ |______| |      | |_______     |        \_\    \_/    _
|______| |      | |______| _______|     |           \____\____|_|
```

# ghostmodule

Lightweight, multiplatform and accessible framework for command line-based programs and C++ microservices.

|         Build system         |                         Build status                         |
| :--------------------------: | :----------------------------------------------------------: |
|      Windows x64 VS2015      | [![Build status](https://ci.appveyor.com/api/projects/status/urayu2uebhqq3m6y?svg=true)](https://ci.appveyor.com/project/mathieunassar/ghostmodule) |
| Linux x64 (xenial) GCC 5.4.0 | [![Build Status](https://travis-ci.com/mathieunassar/ghostmodule.svg?branch=master)](https://travis-ci.com/mathieunassar/ghostmodule) |
|    MacOS x64 apple-clang     | [![Build Status](https://travis-ci.com/mathieunassar/ghostmodule.svg?branch=master)](https://travis-ci.com/mathieunassar/ghostmodule) |

## Overview

ghostmodule is a C++ library providing a framework for simple to complex command line-based applications and microservices. It provides the following functionalities:

- **Program lifetime management**: separates the different stages of the program's life: its initialization, runtime and finalization;
- **Console control**: provides a way to input commands, and controls the output flow to pause when the User is typing;
- **Command interpretation**: optionally processes user input as commands, previously defined by the developer;
- **User management**: exposes a login system to restrict the access to some commands and program features;
- **Data persistence**: provides a sub-library (ghost_persistence) based on Google's Protobuf to store data into save files;
- **Connectivity and messaging:** provides sub-libraries (ghost_connection, ghost_connection_grpc, ghost_connection_extension) to exchange messages within your programs and beyond. Define the transported data, ghostmodule does the rest;
- **Multiplatform**: the following platforms are officially supported:
  - Linux (Ubuntu Xenial, GCC compilers);
  - Windows (MSVC compilers);
  - OSX (Apple clang).

This README file contains the installation instructions, as well as a brief introduction to the features provided by ghostmodule.

## Setup

Being lightweight, the installation of ghostmodule is simple and straightforward.

ghostmodule uses CMake (<https://cmake.org/>) and Conan (<https://conan.io/>) for the dependency management.

The following external dependencies are used by this project:

- **Googletest** (<https://github.com/google/googletest>): used for unit testing;
- **Google Protobuf** (<https://github.com/protocolbuffers/protobuf>): used by "ghost_persistence" to serialize and store data into storage files;
- **Google gRPC** (https://github.com/grpc): used by "ghost_connection_grpc" to transport messages over the networked HTTP2 protocol.

It is possible to build (parts of) the project without any of these dependencies.

When building requires one of these dependencies, **Conan** must be installed and "`conan`" must be callable from a bash console. It can be easily installed with Python3's pip install tool or from Homebrew for OSX hosts (https://brew.sh).

#### Simple installation (without unit tests, with examples)

Per default, building the unit tests and the examples is disabled. To build the library, simply execute the CMake script, as in the following example for Windows:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON -G "Visual Studio 15 Win64"
cmake --build . --config Release
```

For debug builds, with a default generator:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_EXAMPLES=ON
cmake --build .
```

If "`BUILD_EXAMPLES=True`" is not specified, the examples will not be built.

*Note: during the configure phase of CMake, a support repository is downloaded from Github, which contains implementation details: ghostsupport.*

#### Custom installation

The following options can be set to customize the build:

| Option                   | Description                                                  | Default |
| ------------------------ | ------------------------------------------------------------ | ------- |
| **BUILD_TESTS**          | if set to "ON", the unit tests will be built.                | OFF     |
| **BUILD_SYSTEMTESTS**    | if set to "ON", the system tests will be built.              | OFF     |
| **BUILD_EXAMPLES**       | if set to "ON", example programs will be built.              | OFF     |
| **BUILD_MODULE***        | if set to "ON", the library "ghost_module" will be built.    | ON      |
| **BUILD_PERSISTENCE**    | if set to "ON", the library "ghost_persistence" will be built. | ON      |
| **BUILD_CONNECTION**     | if set to "ON", the library "ghost_connection" will be built. | ON      |
| **BUILD_CONNECTIONGRPC** | if set to "ON", the library "ghost_connection_grpc" will be built. | ON      |

*: Building the "ghost_module" library only does not require any dependency, see the "Setup" section.

The following example build the whole repository and executes the unit tests (with `ctest .`)

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_SYSTEMTESTS=ON -DBUILD_EXAMPLES=ON
cmake --build .
ctest .
```

*Note: multi-configurations generators is not supported by this project. In order to switch from a Debug build to a Release build (or the contrary), cmake must be executed again with the corresponding value for the parameter "CMAKE_BUILD_TYPE".*

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
- examples/connection_grpc_messenger.cpp: a classic and simplistic implementation of a messenger program. "**Connectivity and messaging**" is used through the "ghost_connection" and its specialization for networked connections "ghost_connection_grpc" to illustrate the message exchange between a chat server and client.
- examples/connection_grpc_robot.cpp: This example gets closer to common uses of distributed system in that it simulates a (very simplified) robot that can move with a fixed velocity and provide its current odometry. While the "**console control**" feature is used to manipulate the velocity of the robot, this example emphasizes the usage of "**Connectivity and messaging**" to illustrate the odometry provision.
- examples/connection_extension_daemon.cpp: the library ghost_connection_extension integrates the features of ghost_connection into the library ghost_module. This example shows how to use this sub-library to create a daemon / service module, accessible by a remote client. That is, it illustrates the features "**command interpretation**" and "**console control**" together with the "**Connectivity and messaging**".

In order to build the examples, pass the flag "`-DBUILD_EXAMPLES=True`" to CMake. After quickly inspecting the source code, you can execute the programs and play around with them.

To continue working with this library, please read the documentation contained in the API header files contained in the folders include/ghost/module and include/ghost/persistence.

## Feedback

If you are using this library, find a bug, have a recommendation or anything else, I would be glad to hear from you!
