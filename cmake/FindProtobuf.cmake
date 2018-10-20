SET(Protobuf_INCLUDE_DIRS "${GHOST_CONNECTION_ROOT_DIR}/third-party/protobuf/include" CACHE STRING "Protobuf include directory")

find_library(
	Protobuf_LIBRARIES_RELEASE
	NAMES protobuf libprotobuf
	PATHS "${GHOST_CONNECTION_ROOT_DIR}/third-party/protobuf/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	Protobuf_LIBRARIES_DEBUG
	NAMES protobuf libprotobufd protobufd libprotobufd
	PATHS "${GHOST_CONNECTION_ROOT_DIR}/third-party/protobuf/bin/${GHOST_BUILD_NAME}/Debug"
)

if (${Protobuf_LIBRARIES_RELEASE} MATCHES "Protobuf_LIBS_RELEASE-NOTFOUND")
	message(SEND_ERROR "Protobuf not found!")
	unset(Protobuf_INCLUDE_DIRS)
else ()
	message(STATUS "Found Protobuf: ")
	message(STATUS "- include: " ${Protobuf_INCLUDE_DIRS})
	message(STATUS "- lib (release): " ${Protobuf_LIBRARIES_RELEASE})
	message(STATUS "- lib (debug): " ${Protobuf_LIBRARIES_DEBUG})
	
	set(Protobuf_LIBRARIES
		optimized ${Protobuf_LIBRARIES_RELEASE}
		debug ${Protobuf_LIBRARIES_DEBUG}
	)
endif ()
