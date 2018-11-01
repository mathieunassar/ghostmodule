file(READ ${GHOST_PERSISTENCE_ROOT_DIR}/third-party/protobuf_version.txt REQUIRED_VERSION_PROTOBUF)
set(PROTOBUF_VERSION_FOLDER "protobuf_${REQUIRED_VERSION_PROTOBUF}")

SET(Protobuf_INCLUDE_DIRS "${GHOST_THIRD_PARTIES_ROOT_DIR}/protobuf/${PROTOBUF_VERSION_FOLDER}/include" CACHE STRING "Protobuf include directory")

find_library(
	Protobuf_LIBRARIES_RELEASE
	NAMES protobuf libprotobuf
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/protobuf/${PROTOBUF_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release"
)

find_library(
	Protobuf_LIBRARIES_DEBUG
	NAMES protobuf libprotobufd protobufd libprotobufd
	PATHS "${GHOST_THIRD_PARTIES_ROOT_DIR}/protobuf/${PROTOBUF_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Debug"
)

if (WIN32)
	set(PROTOC_PATH ${GHOST_THIRD_PARTIES_ROOT_DIR}/protobuf/${PROTOBUF_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release/protoc.exe CACHE FILEPATH "Path to the Protoc compiler")
endif (WIN32)

if (UNIX)
	set(PROTOC_PATH ${GHOST_THIRD_PARTIES_ROOT_DIR}/protobuf/${PROTOBUF_VERSION_FOLDER}/bin/${GHOST_BUILD_NAME}/Release/protoc CACHE FILEPATH "Path to the Protoc compiler")
endif (UNIX)


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
