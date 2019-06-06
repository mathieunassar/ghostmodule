cmake_minimum_required(VERSION 3.8)

set(projectName ghost_module)
set(GHOST_MODULE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE FILEPATH "Root directory of ghost_module")
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

# define the project
project(${projectName} CXX)

# setup ghost variables
include(cmake/ghost.cmake)
setup_ghost_repository()

# Windows specific stuff
if (MSVC)
	message(STATUS "MSVC compiler detected - version: " ${MSVC_VERSION})
	
	if (MSVC_VERSION GREATER_EQUAL "1900")
		include(CheckCXXCompilerFlag)
		CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
		if (_cpp_latest_flag_supported)
			add_compile_options("/std:c++latest")
		endif()
	endif()
	
	message(STATUS "Setting D_WIN32_WINNT to 0x600")
	add_compile_options("-D_WIN32_WINNT=0x600")
	
	message(STATUS "Setting /EHsc flag")
	add_compile_options("/EHsc")
endif (MSVC)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# import Ghost Support
SET(GHOST_SUPPORT_URL "https://github.com/mathieunassar/ghostsupport" CACHE STRING "Ghost Support location")
file(READ ${GHOST_MODULE_ROOT_DIR}/third-party/ghost_support_version.txt GHOST_SUPPORT_BRANCH)
fetch_ghost_library(ghost_support ${GHOST_SUPPORT_URL} ${GHOST_SUPPORT_BRANCH})

#find google test
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

##########################################################################################################################################
######################################################## INCLUDE DIRECTORIES AND LINK ####################################################
##########################################################################################################################################

SET(GHOST_MODULE_INCLUDE_DIRECTORIES
	${GHOST_MODULE_ROOT_DIR}/include
	${GHOST_SUPPORT_INCLUDE_DIRECTORIES}
	CACHE FILEPATH "Include directories of ghost_module"
)

# provide path to include directories
include_directories(
	${GHOST_MODULE_INCLUDE_DIRECTORIES}
	${Googletest_INCLUDE_DIRS}
)

##########################################################################################################################################
################################################################ MODULE LIBRARY ##########################################################
##########################################################################################################################################

file(GLOB header_module_lib
./include/ghost/module/Console.hpp
./include/ghost/module/CommandLineInterpreter.hpp
./include/ghost/module/Command.hpp
./include/ghost/module/CommandLine.hpp
./include/ghost/module/PermissionEntity.hpp
./include/ghost/module/User.hpp
./include/ghost/module/UserGroup.hpp
./include/ghost/module/UserManager.hpp
./include/ghost/module/Module.hpp
./include/ghost/module/ModuleBuilder.hpp
./include/ghost/module/Logger.hpp
./include/ghost/module/LoggerLine.hpp
./include/ghost/module/GhostLogger.hpp
./include/ghost/module/StdoutLogger.hpp
)

file(GLOB header_module_lib_internal_windows
./src/ConsoleDeviceWindows.hpp
)

file(GLOB header_module_lib_internal_unix
./src/ConsoleDeviceUnix.hpp
)

file(GLOB header_module_lib_internal
./src/Console.hpp
./src/Prompt.hpp
./src/InputController.hpp
./src/InputControllerAccess.hpp
./src/ConsoleDevice.hpp
./src/ConsoleStream.hpp
./src/InputEvent.hpp
./src/EnterPressedInputEvent.hpp
./src/InputModeInputEvent.hpp
./src/LineRequestInputEvent.hpp
./src/OutputController.hpp
./src/CommandLineInterpreter.hpp
./src/CommandLineParser.hpp
./src/User.hpp
./src/UserGroup.hpp
./src/UserManager.hpp
./src/Module.hpp
./src/ModuleBuilder.hpp
./src/GhostLogger.hpp
./src/StdoutLogger.hpp
)

if (WIN32)
	set(header_module_lib_internal ${header_module_lib_internal} ${header_module_lib_internal_windows})
else()
	set(header_module_lib_internal ${header_module_lib_internal} ${header_module_lib_internal_unix})
endif()

file(GLOB header_module_lib_internal_commands
./src/commands/LoginCommand.hpp
./src/commands/HelpCommand.hpp
./src/commands/ExitCommand.hpp
)

file(GLOB source_module_lib_internal_windows
./src/ConsoleDeviceWindows.cpp
)

file(GLOB source_module_lib_internal_unix
./src/ConsoleDeviceUnix.cpp
)

file(GLOB source_module_lib
./src/Console.cpp
./src/Prompt.cpp
./src/InputController.cpp
./src/OutputController.cpp
./src/EnterPressedInputEvent.cpp
./src/InputModeInputEvent.cpp
./src/LineRequestInputEvent.cpp
./src/CommandLine.cpp
./src/CommandLineParser.cpp
./src/CommandLineInterpreter.cpp
./src/User.cpp
./src/UserGroup.cpp
./src/UserManager.cpp
./src/Module.cpp
./src/ModuleBuilder.cpp
./src/GhostLogger.cpp
./src/StdoutLogger.cpp
./src/LoggerLine.cpp
./src/Logger.cpp
#./src/main.cpp
)

if (WIN32)
	set(source_module_lib ${source_module_lib} ${source_module_lib_internal_windows})
else()
	set(source_module_lib ${source_module_lib} ${source_module_lib_internal_unix})
endif()

file(GLOB source_module_lib_commands
./src/commands/LoginCommand.cpp
./src/commands/HelpCommand.cpp
./src/commands/ExitCommand.cpp
)


source_group("API" FILES ${header_module_lib})
source_group("Header Files\\commands" FILES ${header_module_lib_internal_commands})
source_group("Source Files\\commands" FILES ${source_module_lib_commands})

##########################################################################################################################################

add_library(ghost_module ${header_module_lib}
	${source_module_lib}
	${header_module_lib_internal}
	${header_module_lib_internal_commands}
	${source_module_lib_commands})

target_link_libraries(ghost_module)

if (UNIX)
	target_link_libraries(ghost_module pthread)
endif()

##### Unit tests #####

file(GLOB module_tests_src
	tests/CommandLineInterpreterTests.cpp
	tests/UserManagerTests.cpp
	tests/ModuleTests.cpp
)

add_executable(module_tests ${module_tests_src})
target_link_libraries(module_tests ghost_module ${CONAN_LIBS})

set_property(TARGET module_tests PROPERTY FOLDER "tests")

##### Examples #####

# minimum code to create a module
add_executable(module_minimum examples/module_minimum.cpp)
target_link_libraries(module_minimum ghost_module)

set_property(TARGET module_minimum PROPERTY FOLDER "examples")

# module that writes every second where the user can update the written text
add_executable(module_with_console examples/module_with_console.cpp)
target_link_libraries(module_with_console ghost_module)

set_property(TARGET module_with_console PROPERTY FOLDER "examples")
