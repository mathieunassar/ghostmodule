file(READ ${GHOST_CONNECTION_ROOT_DIR}/third-party/grpc_version.txt REQUIRED_VERSION_GRPC)
set(GRPC_VERSION_FOLDER "grpc_${REQUIRED_VERSION_GRPC}")

SET(GRPC_INCLUDE_DIRS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/include" CACHE STRING "gRPC include directory")

find_library(
	GRPC_GPR_RELEASE
	NAMES libgpr.a gpr
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GRPC_RELEASE
	NAMES libgrpc.a grpc
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GRPC++_RELEASE
	NAMES libgrpc++.a grpc++
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GRPC_UNSECURE_RELEASE
	NAMES libgrpc_unsecure.a grpc_unsecure
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GRPC++_UNSECURE_RELEASE
	NAMES libgrpc++_unsecure.a grpc++_unsecure
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_Z_RELEASE
	NAMES libz.a zlib
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_ADDRESS_SORTING_RELEASE
	NAMES libaddress_sorting.a address_sorting
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_CARES_RELEASE
	NAMES cares ares
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GPR_DEBUG
	NAMES libgpr.a gpr
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_GRPC_DEBUG
	NAMES libgrpc.a grpc
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_GRPC++_DEBUG
	NAMES libgrpc++.a grpc++ 
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_GRPC_UNSECURE_DEBUG
	NAMES libgrpc_unsecure.a grpc_unsecure
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_GRPC++_UNSECURE_DEBUG
	NAMES libgrpc++_unsecure.a grpc++_unsecure
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_Z_DEBUG
	NAMES libz.a zlib zlibd
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_ADDRESS_SORTING_DEBUG
	NAMES address_sorting
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_CARES_DEBUG
	NAMES cares ares
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

if (WIN32)
	set(GRPC_CPP_PLUGIN_PATH ${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release/grpc_cpp_plugin.exe CACHE FILEPATH "Path to the gRPC plugin for protoc")
endif (WIN32)

if (UNIX)
	set(GRPC_CPP_PLUGIN_PATH ${GHOST_THIRD_PARTIES_ROOT_DIR}/grpc/${GRPC_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release/grpc_cpp_plugin CACHE FILEPATH "Path to the gRPC plugin for protoc")
endif (UNIX)

if (${GRPC_GPR_RELEASE} MATCHES "GRPC_GPR_RELEASE-NOTFOUND")
	#OR ${GRPC_GRPC_RELEASE} MATCHES "GRPC_GRPC_RELEASE-NOTFOUND"
#	OR ${GRPC_GRPC++_RELEASE} MATCHES "GRPC_GRPC++_RELEASE-NOTFOUND")

	message(SEND_ERROR "GRPC not found!")
	unset(GRPC_INCLUDE_DIRS)
else ()
	message(STATUS "Found GRPC: ")
	message(STATUS "- include: " ${GRPC_INCLUDE_DIRS})
	message(STATUS "- lib gpr (release): " ${GRPC_GPR_RELEASE})
	message(STATUS "- lib grpc (release): " ${GRPC_GRPC_RELEASE})
	message(STATUS "- lib grpc++ (release): " ${GRPC_GRPC++_RELEASE})
	message(STATUS "- lib grpc_unsecure (release): " ${GRPC_GRPC_UNSECURE_RELEASE})
	message(STATUS "- lib grpc++_unsecure (release): " ${GRPC_GRPC++_UNSECURE_RELEASE})
	message(STATUS "- lib address_sorting (release): " ${GRPC_ADDRESS_SORTING_RELEASE})
	message(STATUS "- lib Z (release): " ${GRPC_Z_RELEASE})
	message(STATUS "- lib Cares (release): " ${GRPC_CARES_RELEASE})
	
	message(STATUS "- lib gpr (debug): " ${GRPC_GPR_DEBUG})
	message(STATUS "- lib grpc (debug): " ${GRPC_GRPC_DEBUG})
	message(STATUS "- lib grpc++ (debug): " ${GRPC_GRPC++_DEBUG})
	message(STATUS "- lib grpc_unsecure (debug): " ${GRPC_GRPC_UNSECURE_DEBUG})
	message(STATUS "- lib grpc++_unsecure (debug): " ${GRPC_GRPC++_UNSECURE_DEBUG})
	message(STATUS "- lib address_sorting (debug): " ${GRPC_ADDRESS_SORTING_DEBUG})
	message(STATUS "- lib Z (debug): " ${GRPC_Z_DEBUG})
	message(STATUS "- lib Cares (debug): " ${GRPC_CARES_DEBUG})

	set(GRPC_LIBRARIES_RELEASE ${GRPC_GPR_RELEASE} ${GRPC_GRPC_RELEASE} ${GRPC_GRPC++_RELEASE} ${GRPC_Z_RELEASE} ${GRPC_CARES_RELEASE} ${GRPC_ADDRESS_SORTING_RELEASE} ${GRPC_GRPC_UNSECURE_RELEASE} ${GRPC_GRPC++_UNSECURE_RELEASE})

	set(GRPC_LIBRARIES_DEBUG ${GRPC_GPR_DEBUG} ${GRPC_GRPC_DEBUG} ${GRPC_GRPC++_DEBUG} ${GRPC_Z_DEBUG} ${GRPC_CARES_DEBUG} ${GRPC_ADDRESS_SORTING_DEBUG} ${GRPC_GRPC_UNSECURE_DEBUG} ${GRPC_GRPC++_UNSECURE_DEBUG})
	
	set(GRPC_LIBRARIES
		optimized ${GRPC_GPR_RELEASE}
		optimized ${GRPC_GRPC_RELEASE}
		optimized ${GRPC_GRPC++_RELEASE}
		optimized ${GRPC_Z_RELEASE}
		optimized ${GRPC_CARES_RELEASE}
		optimized ${GRPC_ADDRESS_SORTING_RELEASE}
		optimized ${GRPC_GRPC_UNSECURE_RELEASE}
		optimized ${GRPC_GRPC++_UNSECURE_RELEASE}
		debug ${GRPC_GPR_DEBUG}
		debug ${GRPC_GRPC_DEBUG}
		debug ${GRPC_GRPC++_DEBUG}
		debug ${GRPC_Z_DEBUG}
		debug ${GRPC_CARES_DEBUG}
		debug ${GRPC_ADDRESS_SORTING_DEBUG}
		debug ${GRPC_GRPC_UNSECURE_DEBUG}
		debug ${GRPC_GRPC++_UNSECURE_DEBUG}
	)
endif ()
