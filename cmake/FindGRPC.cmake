SET(GRPC_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/third-party/grpc/include" CACHE STRING "gRPC include directory")

find_library(
	GRPC_GPR_RELEASE
	NAMES libgpr.a gpr
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GRPC_RELEASE
	NAMES libgrpc.a grpc
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GRPC++_RELEASE
	NAMES libgrpc++.a grpc++
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GRPC_UNSECURE_RELEASE
	NAMES libgrpc_unsecure.a grpc_unsecure
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GRPC++_UNSECURE_RELEASE
	NAMES libgrpc++_unsecure.a grpc++_unsecure
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_Z_RELEASE
	NAMES libz.a zlib
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	GRPC_GPR_DEBUG
	NAMES libgpr.a gpr
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_GRPC_DEBUG
	NAMES libgrpc.a grpc
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_GRPC++_DEBUG
	NAMES libgrpc++.a grpc++ 
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_GRPC_UNSECURE_DEBUG
	NAMES libgrpc_unsecure.a grpc_unsecure
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_GRPC++_UNSECURE_DEBUG
	NAMES libgrpc++_unsecure.a grpc++_unsecure
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Debug"
)

find_library(
	GRPC_Z_DEBUG
	NAMES libz.a zlib
	PATHS "${CMAKE_SOURCE_DIR}/third-party/grpc/bin/${GHOST_BUILD_NAME}/Debug"
)

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
	message(STATUS "- lib Z (release): " ${GRPC_Z_RELEASE})

	message(STATUS "- lib gpr (debug): " ${GRPC_GPR_DEBUG})
	message(STATUS "- lib grpc (debug): " ${GRPC_GRPC_DEBUG})
	message(STATUS "- lib grpc++ (debug): " ${GRPC_GRPC++_DEBUG})
	message(STATUS "- lib grpc_unsecure (debug): " ${GRPC_GRPC_UNSECURE_DEBUG})
	message(STATUS "- lib grpc++_unsecure (debug): " ${GRPC_GRPC++_UNSECURE_DEBUG})
	message(STATUS "- lib Z (debug): " ${GRPC_Z_DEBUG})

	set(GRPC_LIBRARIES_RELEASE ${GRPC_GPR_RELEASE} ${GRPC_GRPC_RELEASE} ${GRPC_GRPC++_RELEASE} ${GRPC_Z_RELEASE} ${GRPC_GRPC_UNSECURE_RELEASE} ${GRPC_GRPC++_UNSECURE_RELEASE})

	set(GRPC_LIBRARIES_DEBUG ${GRPC_GPR_DEBUG} ${GRPC_GRPC_DEBUG} ${GRPC_GRPC++_DEBUG} ${GRPC_Z_DEBUG} ${GRPC_GRPC_UNSECURE_DEBUG} ${GRPC_GRPC++_UNSECURE_DEBUG})
endif ()
