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


# Windows specific stuff
if (MSVC)
	set(GHOST_BUILD_NAME "buildVS2017_x86")
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

# unix specific stuff
if (UNIX)
    set(GHOST_BUILD_NAME "buildUnix_x86_64")
endif (UNIX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

##########################################################################################################################################
######################################################## INCLUDE DIRECTORIES AND LINK ####################################################
##########################################################################################################################################

# provide path to include directories
include_directories(
	include
	third-party/ghost_support/include
)

# provide path to libraries
link_directories(
)

# set up tests
enable_testing()

ADD_SUBDIRECTORY(third-party/ghost_support)

##########################################################################################################################################
################################################################ MODULE LIBRARY ##########################################################
##########################################################################################################################################

file(GLOB header_module_lib
./include/Console.hpp
./include/InputController.hpp
./include/OutputController.hpp
./include/CommandLineInterpreter.hpp
./include/Command.hpp
./include/CommandLine.hpp
./include/PermissionEntity.hpp
./include/User.hpp
./include/UserGroup.hpp
./include/UserManager.hpp
./include/Module.hpp
)

file(GLOB header_module_lib_internal_windows
./include/internal/ConsoleDeviceWindows.hpp
)

file(GLOB header_module_lib_internal_unix
./include/internal/ConsoleDeviceUnix.hpp
)

file(GLOB header_module_lib_internal
./include/internal/Console.hpp
./include/internal/Prompt.hpp
./include/internal/InputController.hpp
./include/internal/InputControllerAccess.hpp
./include/internal/ConsoleDevice.hpp
./include/internal/ConsoleStream.hpp
./include/internal/InputEvent.hpp
./include/internal/EnterPressedInputEvent.hpp
./include/internal/InputModeInputEvent.hpp
./include/internal/LineRequestInputEvent.hpp
./include/internal/OutputController.hpp
./include/internal/CommandLineInterpreter.hpp
./include/internal/CommandLineParser.hpp
./include/internal/User.hpp
./include/internal/UserGroup.hpp
./include/internal/UserManager.hpp
./include/internal/Module.hpp
)

if (WIN32)
	set(header_module_lib_internal ${header_module_lib_internal} ${header_module_lib_internal_windows})
else()
	set(header_module_lib_internal ${header_module_lib_internal} ${header_module_lib_internal_unix})
endif()

file(GLOB header_module_lib_internal_commands
./include/internal/commands/LoginCommand.hpp
./include/internal/commands/HelpCommand.hpp
./include/internal/commands/ExitCommand.hpp
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
./src/main.cpp)

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


source_group("Header Files\\internal" FILES ${header_module_lib_internal})
source_group("Header Files\\internal\\commands" FILES ${header_module_lib_internal_commands})
source_group("Source Files\\commands" FILES ${source_module_lib_commands})

##########################################################################################################################################

add_executable(ghost_module ${header_module_lib}
	${source_module_lib}
	${header_module_lib_internal}
	${header_module_lib_internal_commands}
	${source_module_lib_commands})

target_link_libraries(ghost_module)

if (UNIX)
	target_link_libraries(ghost_module pthread)
endif()
