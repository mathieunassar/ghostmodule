cmake_minimum_required(VERSION 3.8)

set(projectName ghost_persistence)
set(GHOST_PERSISTENCE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE FILEPATH "Root directory of ghost_persistence")
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${GHOST_PERSISTENCE_ROOT_DIR}/cmake/")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	message(STATUS "build ${projectName} as root project")
endif()

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

SET(GHOST_THIRD_PARTIES_ROOT_DIR ${GHOST_PERSISTENCE_ROOT_DIR}/../ghost_third_parties CACHE STRING "Ghost Third Parties location")

find_package(Protobuf REQUIRED)
find_package(Catch REQUIRED)

include(cmake/ProtobufGeneration.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

##########################################################################################################################################
######################################################## INCLUDE DIRECTORIES AND LINK ####################################################
##########################################################################################################################################

# provide path to include directories
include_directories(
	include
	${Protobuf_INCLUDE_DIRS}
	${CATCH_INCLUDE_DIRS}
)

# provide path to libraries
link_directories(
	${Protobuf_LIBS_RELEASE}
)

# set up tests
enable_testing()

##########################################################################################################################################
############################################################ CONNECTION LIBRARY ##########################################################
##########################################################################################################################################

##########################################################################################################################################

file(GLOB header_persistence_lib
./include/ghost/persistence/SaveData.hpp
./include/ghost/persistence/SaveManager.hpp
)

file(GLOB header_persistence_internal_lib
./include/ghost/persistence/internal/SaveFile.hpp
./include/ghost/persistence/internal/SaveData.hpp
./include/ghost/persistence/internal/SaveManager.hpp
)

file(GLOB source_persistence_lib
./src/SaveFile.cpp
./src/SaveData.cpp
./src/SaveManager.cpp
)

source_group("Header Files\\internal" FILES ${header_persistence_internal_lib})

##########################################################################################################################################

add_library(ghost_persistence ${header_persistence_internal_lib}
	${header_persistence_lib}
	${source_persistence_lib}
)

target_link_libraries(ghost_persistence ${Protobuf_LIBRARIES})

##########################################################################################################################################

set(PROTOC_SOURCE_CUSTOM ./tests/protobuf/)
set(PROTOC_SOURCE_FILE ./tests/protobuf/tests.proto)
set(PROTOC_OUTPUT ./tests/protobuf/)

generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})

# Unit tests
file(GLOB source_persistence_tests
./tests/SaveFileTests.cpp
./tests/SaveDataTests.cpp
./tests/SaveManagerTests.cpp
./tests/PersistenceTestHelpers.cpp
./tests/PersistenceTestHelpers.hpp
)

file(GLOB protobuf_persistence_tests
./tests/protobuf/tests.pb.h
./tests/protobuf/tests.pb.cc
)

set(libraries_persistence_tests
ghost_persistence)

add_executable(persistence_tests ${source_persistence_tests} ${protobuf_persistence_tests})
target_link_libraries(persistence_tests ${libraries_persistence_tests})
set_property(TARGET persistence_tests PROPERTY FOLDER "tests")
add_test(persistence_tests persistence_tests)
