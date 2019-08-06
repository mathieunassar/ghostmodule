##########################################################################################################################################
############################################################ CONNECTION LIBRARY ##########################################################
##########################################################################################################################################

# Protobuf files generation
set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/protobuf/)
set(PROTOC_SOURCE_FILE ${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/Ghost.proto)
set(PROTOC_OUTPUT ${GHOST_MODULE_ROOT_DIR}/protobuf/)

generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})

# targets defintion
file(GLOB header_connection_lib
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Connection.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Server.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/ClientHandler.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/MessageHandler.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Client.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Publisher.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Subscriber.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Message.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Configuration.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/ConnectionConfiguration.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/NetworkConnectionConfiguration.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/ConnectionManager.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/ConnectionFactory.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/ConnectionFactory.impl.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Writer.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Reader.hpp
)

file(GLOB header_connection_lib_internal
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/Connection.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/Connection.impl.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/GenericMessageConverter.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ProtobufMessage.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ProtobufConfiguration.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/MessageHandler.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/MessageHandlerCallback.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ConnectionManager.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ConnectionFactory.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ConnectionFactory.impl.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ConnectionFactoryRule.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ConnectionFactoryRule.impl.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/WriterSink.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ReaderSink.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/QueuedSink.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/GenericWriter.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/GenericWriter.impl.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/GenericReader.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/GenericReader.impl.hpp
)

file(GLOB header_connection_lib_internal_network
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/ServerGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/BaseClientGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/BaseClientGRPC.impl.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/ClientGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/RemoteClientGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/PublisherGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/PublisherClientHandler.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/SubscriberGRPC.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/CompletionQueueExecutor.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/RPCStateMachine.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/network/ClientManager.hpp
)

file(GLOB source_connection_lib_network
${GHOST_MODULE_ROOT_DIR}/src/connection/network/ServerGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/network/ClientGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/network/RemoteClientGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/network/PublisherGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/network/PublisherClientHandler.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/network/SubscriberGRPC.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/network/CompletionQueueExecutor.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/network/RPCStateMachine.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/network/ClientManager.cpp
)

file(GLOB protobuf_connection_lib
${GHOST_MODULE_ROOT_DIR}/protobuf/Ghost.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/Ghost.grpc.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/Ghost.pb.cc
${GHOST_MODULE_ROOT_DIR}/protobuf/Ghost.grpc.pb.cc
)

file(GLOB source_connection_lib
${GHOST_MODULE_ROOT_DIR}/src/connection/Connection.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Server.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Client.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Publisher.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Subscriber.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ProtobufMessage.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/MessageHandler.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/GenericMessageConverter.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Configuration.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionConfiguration.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/NetworkConnectionConfiguration.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ProtobufConfiguration.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionManager.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionFactory.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionFactoryRule.cpp
)

source_group("API" FILES ${header_connection_lib})
source_group("Header Files\\internal" FILES ${header_connection_lib_internal})
source_group("Header Files\\internal\\network" FILES ${header_connection_lib_internal_network})
source_group("Source Files\\network" FILES ${source_connection_lib_network})
source_group("Protobuf" FILES ${protobuf_connection_lib})

##########################################################################################################################################

add_library(ghost_connection ${header_connection_lib}
	${header_connection_lib_internal}
	${header_connection_lib_internal_network}
	${source_connection_lib_network}
	${source_connection_lib}
	${protobuf_connection_lib}
	)

if (UNIX)
	target_link_libraries(ghost_connection pthread)
endif()

target_link_libraries(ghost_connection ${CONAN_LIBS_GRPC} ${CONAN_LIBS_PROTOBUF} ${CONAN_LIBS_C-ARES} ${CONAN_LIBS_ZLIB})

##### Unit tests #####

if ((DEFINED BUILD_TESTS) AND (${BUILD_TESTS}))
	# Unit tests: Primitive Serialization
	file(GLOB source_connection_tests
		${GHOST_MODULE_ROOT_DIR}/tests/ConnectionMocks.hpp
		${GHOST_MODULE_ROOT_DIR}/tests/ConnectionMocks.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/MessageTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/ConfigurationTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/ReaderWriterTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/ConnectionTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/NetworkConnectionTests.cpp)

	add_executable(connection_tests ${source_connection_tests})
	target_link_libraries(connection_tests ghost_connection ${CONAN_LIBS_GTEST})
	
	gtest_add_tests(TARGET connection_tests)

	set_property(TARGET connection_tests PROPERTY FOLDER "tests")
endif()

##### Examples #####

# if ((DEFINED BUILD_EXAMPLES) AND (${BUILD_EXAMPLES}))
# 	set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/)
# 	set(PROTOC_SOURCE_FILE ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/persistency_todo_list.proto)
# 	set(PROTOC_OUTPUT ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/)

# 	generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})

# 	# minimum code to create a module
# 	add_executable(persistence_todo_list
# 		${GHOST_MODULE_ROOT_DIR}/examples/persistence_todo_list.cpp
# 		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/persistency_todo_list.pb.h
# 		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/persistency_todo_list.pb.cc)
# 	target_link_libraries(persistence_todo_list ghost_module ghost_persistence)

# 	set_property(TARGET persistence_todo_list PROPERTY FOLDER "examples")
# endif()
