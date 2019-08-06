cmake_minimum_required(VERSION 3.8)

set(projectName ghostmodule)
set(GHOST_MODULE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE FILEPATH "Root directory of ghostmodule")
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# define the project
project(${projectName} CXX)
enable_testing()
include(GoogleTest)

# setup ghost variables
include(${GHOST_MODULE_ROOT_DIR}/cmake/ghost.cmake)

# fetch the dependencies with conan
if (((DEFINED BUILD_TESTS) AND (${BUILD_TESTS}))
	OR (NOT DEFINED BUILD_PERSISTENCE) OR (${BUILD_PERSISTENCE})
	OR (NOT DEFINED BUILD_CONNECTION) OR (${BUILD_CONNECTION}))
	include(${GHOST_MODULE_ROOT_DIR}/cmake/conan.cmake)
	conan_check(REQUIRED)
	conan_cmake_run(CONANFILE conanfile.txt
					BASIC_SETUP
					BUILD missing)

	include(${GHOST_MODULE_ROOT_DIR}/cmake/generate_protobuf.cmake)
endif()

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

##########################################################################################################################################
######################################################## INCLUDE DIRECTORIES AND LINK ####################################################
##########################################################################################################################################

SET(GHOST_MODULE_INCLUDE_DIRECTORIES
	${GHOST_MODULE_ROOT_DIR}/include
	${GHOST_SUPPORT_INCLUDE_DIRECTORIES}
	CACHE FILEPATH "Include directories of ghostmodule"
)

# provide path to include directories
include_directories(
	${GHOST_MODULE_INCLUDE_DIRECTORIES}
)

##########################################################################################################################################
################################################################### TARGETS ##############################################################
##########################################################################################################################################

add_subdirectory(src)
