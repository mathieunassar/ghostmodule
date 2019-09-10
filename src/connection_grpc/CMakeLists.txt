##########################################################################################################################################
############################################################ CONNECTION LIBRARY ##########################################################
##########################################################################################################################################

# Protobuf files generation
set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/protobuf/)
set(PROTOC_SOURCE_FILE ${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection_grpc/ServerClientService.proto)
set(PROTOC_OUTPUT ${GHOST_MODULE_ROOT_DIR}/protobuf/)

generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})

# targets defintion

file(GLOB header_connectiongrpc_lib
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection_grpc/ConnectionGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection_grpc/ConnectionConfigurationGRPC.hpp
)

file(GLOB header_connectiongrpc_internal_lib
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/ServerGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/ClientGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/RemoteClientGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/PublisherGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/PublisherClientHandler.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/SubscriberGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/CompletionQueueExecutor.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/ClientManager.hpp
)

file(GLOB header_connectiongrpc_internal_lib_rpc
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPC.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPC.impl.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCOperation.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCOperation.impl.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCRead.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCWrite.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCRequest.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCConnect.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCFinish.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCServerFinish.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCDone.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/OutgoingRPC.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/IncomingRPC.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCStateMachine.hpp
)

file(GLOB source_connectiongrpc_lib
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/ConnectionGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/ConnectionConfigurationGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/ServerGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/ClientGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/RemoteClientGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/PublisherGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/PublisherClientHandler.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/SubscriberGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/CompletionQueueExecutor.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/ClientManager.cpp
)

file(GLOB source_connectiongrpc_lib_rpc
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/OutgoingRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/IncomingRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection_grpc/rpc/RPCStateMachine.cpp
)

file(GLOB protobuf_connectiongrpc_lib
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection_grpc/ServerClientService.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection_grpc/ServerClientService.grpc.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection_grpc/ServerClientService.pb.cc
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection_grpc/ServerClientService.grpc.pb.cc
)

source_group("API" FILES ${header_connectiongrpc_lib})
source_group("Protobuf" FILES ${protobuf_connectiongrpc_lib})
source_group("Header Files\\RPC" FILES ${header_connectiongrpc_internal_lib_rpc})
source_group("Source Files\\RPC" FILES ${source_connectiongrpc_lib_rpc})
source_group("Protobuf" FILES ${protobuf_connectiongrpc_lib})

##########################################################################################################################################

add_library(ghost_connection_grpc
	${header_connectiongrpc_lib}
	${header_connectiongrpc_internal_lib}
	${source_connectiongrpc_lib}
	${header_connectiongrpc_internal_lib_rpc}
	${source_connectiongrpc_lib_rpc}
	${protobuf_connectiongrpc_lib}
	)

if (UNIX)
	target_link_libraries(ghost_connection_grpc pthread)
endif()

target_link_libraries(ghost_connection_grpc ghost_connection ${CONAN_LIBS_GRPC} ${CONAN_LIBS_PROTOBUF} ${CONAN_LIBS_C-ARES} ${CONAN_LIBS_ZLIB})

##### Unit tests #####

if ((DEFINED BUILD_TESTS) AND (${BUILD_TESTS}))
	file(GLOB source_connection_gprc_tests
		${GHOST_MODULE_ROOT_DIR}/tests/ConnectionGRPCTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/ConnectionTestUtils.hpp
		${GHOST_MODULE_ROOT_DIR}/tests/ConnectionTestUtils.cpp)

	add_executable(connection_grpc_tests ${source_connection_gprc_tests})
	target_link_libraries(connection_grpc_tests ghost_connection_grpc ${CONAN_LIBS_GTEST})
	
	gtest_add_tests(TARGET connection_grpc_tests)

	set_property(TARGET connection_grpc_tests PROPERTY FOLDER "tests")
endif()

##### Examples #####

if ((DEFINED BUILD_EXAMPLES) AND (${BUILD_EXAMPLES}))
	set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/)
	set(PROTOC_SOURCE_FILE
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/connection_grpc_messenger.proto
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/connection_grpc_robot.proto)
	set(PROTOC_OUTPUT ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/)

	generate_protoc("${PROTOC_SOURCE_CUSTOM}" "${PROTOC_OUTPUT}" "${PROTOC_SOURCE_FILE}")

	# usage of servers and clients as a messenger application
	add_executable(connection_grpc_messenger
		${GHOST_MODULE_ROOT_DIR}/examples/connection_grpc_messenger.cpp
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/connection_grpc_messenger.pb.h
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/connection_grpc_messenger.pb.cc)
	target_link_libraries(connection_grpc_messenger ghost_module ghost_connection ghost_connection_grpc)

	set_property(TARGET connection_grpc_messenger PROPERTY FOLDER "examples")

	# usage of publishers and subscribers for a minimalist robot
	add_executable(connection_grpc_robot
		${GHOST_MODULE_ROOT_DIR}/examples/connection_grpc_robot.cpp
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/connection_grpc_robot.pb.h
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/connection_grpc_robot.pb.cc)
	target_link_libraries(connection_grpc_robot ghost_module ghost_connection ghost_connection_grpc)

	set_property(TARGET connection_grpc_robot PROPERTY FOLDER "examples")
endif()
