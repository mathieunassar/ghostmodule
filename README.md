``` ascii
 _______ _      _  ______  ________ _________
|  _____ |______| |      | |_______     |
|______| |      | |______| _______|     |
```

# ghostmodule

Lightweight, multiplatform and accessible framework to create simple command line-based programs.

## Overview

ghostmodule is a C++ library providing a framework for simple command line applications. It provides the following functionalities:

- **Program lifetime management**: separates the different stages of the program's life: its initialization, runtime and finalization;
- **Console control**: provides a way to input commands, and controls the output flow to pause when the User is typing;
- **Command interpretation**: optionally processes user input as commands, previously defined by the developer;
- **User management**: exposes a login system to restrict the access to some commands and program features;
- **Multiplatform**: Linux (Ubuntu Xenial, GCC compilers) and Windows (MSVC compilers) platforms are currently supported.

This README file contains the installation instructions, as well as a brief introduction to the features provided by ghostmodule.

## Setup

Being lightweight, the installation of ghostmodule is simple and straightforward.

ghostmodule uses CMake (<https://cmake.org/>) and Conan (<https://conan.io/>) for the dependency management.

The only external dependency is googletest (<https://github.com/google/googletest>), which is used for the unit tests.

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

The unit tests require googletest (<https://github.com/google/googletest>), which is brought to the project by Conan. **Conan** must be installed, which can be easily installed with Python3's pip install tool. Once "`conan`" call be called from a bash console, building the project is easy:

```bash
mkdir build
cd build
cmake .. -DBUILD_TESTS=True -DBUILD_EXAMPLES=True -G "Visual Studio 15 Win64"
cmake --build .
ctest .
```

*Note: multi-configurations generators is not supported by this project. In order to switch from a Debug build to a Release build (or the contrary), cmake must be executed again with the corresponding value for the parameter "`CMAKE_BUILD_TYPE`".*

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
# The variable GHOST_MODULE_INCLUDE_DIRECTORIES is generated by ghostmodule's CMakeLists
include_directories(${GHOST_MODULE_INCLUDE_DIRECTORIES})
...
# The target "ghost_module" is created by ghostmodule's CMakeLists
target_link_libraries(yourTarget ghost_module)
```

## Getting Started

Two examples are provided to grasp the basic features provided by ghostmodule:

- examples/module_minimum.cpp: The program shows how to create a simple module and illustrates the feature "**program lifetime management**".
- examples/module_with_console.cpp: This example implements a program that cyclically displays a text, and that provides a command to update the text. The program uses the "**console control**" feature to request a user input, and the "**command interpretation**" feature to automatically process the command.

In order to build the examples, pass the flag "`-DBUILD_EXAMPLES=True`" to CMake. After quickly inspecting the source code, you can execute the programs and play around with them.

To continue working with this library, please read the documentation contained in the API header files contained in the folder include/ghost/module.

## Feedback

If you are using this library, find a bug or have a recommendation, I would be glad to hear from you!