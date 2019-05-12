file(READ ${GHOST_MODULE_ROOT_DIR}/third-party/googletest_version.txt REQUIRED_VERSION_GOOGLETEST)
set(GOOGLETEST_VERSION_FOLDER "googletest_${REQUIRED_VERSION_GOOGLETEST}")

SET(Googletest_INCLUDE_DIRS "${GHOST_THIRD_PARTIES_ROOT_DIR}/googletest/${GOOGLETEST_VERSION_FOLDER}/include" CACHE STRING "Googletest include directory")

find_library(
	Googletest_LIBRARIES_RELEASE
	NAMES gtest libgtest
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/googletest/${GOOGLETEST_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	Googletest_main_LIBRARIES_RELEASE
	NAMES gtest_main libgtest_main
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/googletest/${GOOGLETEST_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	Googletest_LIBRARIES_DEBUG
	NAMES gtest libgtestd gtestd libgtestd
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/googletest/${GOOGLETEST_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	Googletest_main_LIBRARIES_DEBUG
	NAMES gtest_main libgtest_maind gtest_maind libgtest_maind
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/googletest/${GOOGLETEST_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

if (${Googletest_LIBRARIES_RELEASE} MATCHES "Googletest_LIBS_RELEASE-NOTFOUND")
	message(SEND_ERROR "Googletest not found!")
	unset(Googletest_INCLUDE_DIRS)
else ()
	message(STATUS "Found Googletest: ")
	message(STATUS "- include: " ${Googletest_INCLUDE_DIRS})
	message(STATUS "- lib (release): " ${Googletest_LIBRARIES_RELEASE})
	message(STATUS "- lib (debug): " ${Googletest_LIBRARIES_DEBUG})
	message(STATUS "- lib with main (release): " ${Googletest_main_LIBRARIES_RELEASE})
	message(STATUS "- lib with main (debug): " ${Googletest_main_LIBRARIES_DEBUG})
	
	set(Googletest_LIBRARIES
		optimized ${Googletest_LIBRARIES_RELEASE}
		debug ${Googletest_LIBRARIES_DEBUG}
	)
	
	set(Googletest_main_LIBRARIES
		optimized ${Googletest_LIBRARIES_RELEASE} optimized ${Googletest_main_LIBRARIES_RELEASE}
		debug ${Googletest_LIBRARIES_DEBUG} debug ${Googletest_main_LIBRARIES_DEBUG}
	)
endif ()
