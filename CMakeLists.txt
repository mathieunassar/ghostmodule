# CONNECTION LIBRARY

#####################################################################

set(PROTOC_SOURCE_CUSTOM ./protobuf/)
set(PROTOC_SOURCE_LIB ${PROJECT_SOURCE_DIR}/ExternalLibs/protobuf/include)
set(PROTOC_SOURCE_FILE ./protobuf/Ghost.proto)
set(PROTOC_OUTPUT ./protobuf/)
set(PROTOC_PATH ${PROJECT_SOURCE_DIR}/ExternalLibs/protobuf/bin/${GHOST_BUILD_NAME}/Release/protoc.exe)
set(GRPC_CPP_PLUGIN_PATH ${PROJECT_SOURCE_DIR}/ExternalLibs/grpc/bin/${GHOST_BUILD_NAME}/Release/grpc_cpp_plugin.exe)

message(STATUS "Generating grpc and protobuf files...")

execute_process(
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${PROTOC_PATH} -I ${PROTOC_SOURCE_LIB} -I ${PROTOC_SOURCE_CUSTOM} --grpc_out=${PROTOC_OUTPUT} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH} ${PROTOC_SOURCE_FILE}
	COMMAND ${PROTOC_PATH} -I ${PROTOC_SOURCE_LIB} -I ${PROTOC_SOURCE_CUSTOM} --cpp_out=${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE}
)

#####################################################################

file(GLOB header_connection2_lib
./include/ghost/connection/Connection.hpp
./include/ghost/connection/Server.hpp
./include/ghost/connection/ClientHandler.hpp
./include/ghost/connection/MessageHandler.hpp
./include/ghost/connection/Client.hpp
./include/ghost/connection/Publisher.hpp
./include/ghost/connection/Subscriber.hpp
./include/ghost/connection/Message.hpp
./include/ghost/connection/ProtobufMessage.hpp
./include/ghost/connection/Configuration.hpp
./include/ghost/connection/ConnectionConfiguration.hpp
./include/ghost/connection/NetworkConnectionConfiguration.hpp
./include/ghost/connection/ConnectionManager.hpp
./include/ghost/connection/ConnectionFactory.hpp
./include/ghost/connection/ConnectionFactory.impl.hpp
./include/ghost/connection/Writer.hpp
./include/ghost/connection/Reader.hpp
)

file(GLOB header_connection2_lib_internal
./include/ghost/connection/internal/Connection.hpp
./include/ghost/connection/internal/Connection.impl.hpp
./include/ghost/connection/internal/GenericMessageConverter.hpp
./include/ghost/connection/internal/ProtobufConfiguration.hpp
./include/ghost/connection/internal/MessageHandler.hpp
./include/ghost/connection/internal/MessageHandlerCallback.hpp
./include/ghost/connection/internal/ConnectionManager.hpp
./include/ghost/connection/internal/ConnectionFactory.hpp
./include/ghost/connection/internal/ConnectionFactory.impl.hpp
./include/ghost/connection/internal/ConnectionFactoryRule.hpp
./include/ghost/connection/internal/ConnectionFactoryRule.impl.hpp
./include/ghost/connection/internal/WriterSink.hpp
./include/ghost/connection/internal/ReaderSink.hpp
./include/ghost/connection/internal/QueuedSink.hpp
./include/ghost/connection/internal/GenericWriter.hpp
./include/ghost/connection/internal/GenericWriter.impl.hpp
./include/ghost/connection/internal/GenericReader.hpp
./include/ghost/connection/internal/GenericReader.impl.hpp
)

file(GLOB header_connection2_lib_internal_network
./include/ghost/connection/internal/network/ServerGRPC.hpp
./include/ghost/connection/internal/network/BaseClientGRPC.hpp
./include/ghost/connection/internal/network/BaseClientGRPC.impl.hpp
./include/ghost/connection/internal/network/ClientGRPC.hpp
./include/ghost/connection/internal/network/RemoteClientGRPC.hpp
./include/ghost/connection/internal/network/PublisherGRPC.hpp
./include/ghost/connection/internal/network/PublisherClientHandler.hpp
./include/ghost/connection/internal/network/SubscriberGRPC.hpp
./include/ghost/connection/internal/network/CompletionQueueExecutor.hpp
./include/ghost/connection/internal/network/RPCStateMachine.hpp
./include/ghost/connection/internal/network/ClientManager.hpp
)

file(GLOB source_connection2_lib_network
./src/network/ServerGRPC.cpp
./src/network/ClientGRPC.cpp
./src/network/RemoteClientGRPC.cpp
./src/network/PublisherGRPC.cpp
./src/network/PublisherClientHandler.cpp
./src/network/SubscriberGRPC.cpp
./src/network/CompletionQueueExecutor.cpp
./src/network/RPCStateMachine.cpp
./src/network/ClientManager.cpp
)

file(GLOB protobuf_connection2_lib
./protobuf/Ghost.pb.h
./protobuf/Ghost.grpc.pb.h
./protobuf/Ghost.pb.cc
./protobuf/Ghost.grpc.pb.cc
)

file(GLOB source_connection2_lib
./src/Connection.cpp
./src/Server.cpp
./src/Client.cpp
./src/Publisher.cpp
./src/Subscriber.cpp
./src/ProtobufMessage.cpp
./src/MessageHandler.cpp
./src/GenericMessageConverter.cpp
./src/Configuration.cpp
./src/ConnectionConfiguration.cpp
./src/NetworkConnectionConfiguration.cpp
./src/ProtobufConfiguration.cpp
./src/ConnectionManager.cpp
./src/ConnectionFactory.cpp
./src/ConnectionFactoryRule.cpp
)

source_group("Header Files\\internal" FILES ${header_connection2_lib_internal})
source_group("Header Files\\internal\\network" FILES ${header_connection2_lib_internal_network})
source_group("Source Files\\network" FILES ${source_connection2_lib_network})
source_group("Protobuf" FILES ${protobuf_connection2_lib})

#####################################################################

add_library(GhostConnection2 ${header_connection2_lib}
	${header_connection2_lib_internal}
	${header_connection2_lib_internal_network}
	${source_connection2_lib_network}
	${source_connection2_lib}
	${protobuf_connection2_lib}
	)

target_link_libraries(GhostConnection2 Commons ws2_32 libprotobuf gpr grpc grpc++ zlib grpc_unsecure grpc++_unsecure)

set_property(TARGET GhostConnection2 PROPERTY FOLDER "Libraries")

#####################################################################

# Unit tests: Primitive Serialization
file(GLOB source_connection2_tests
./tests/ConnectionMocks.hpp
./tests/ConnectionMocks.cpp
./tests/MessageTests.cpp
./tests/ConfigurationTests.cpp
./tests/ReaderWriterTests.cpp
./tests/ConnectionTests.cpp
./tests/NetworkConnectionTests.cpp)

set(libraries_connection2_tests
GhostConnection2)

add_executable(Connection_Tests ${source_connection2_tests})
target_link_libraries(Connection_Tests ${libraries_connection2_tests})
set_property(TARGET Connection_Tests PROPERTY FOLDER "Tests/UnitTests")
add_test(UnitTests_Connection_Tests Connection_Tests)

add_executable(Test1 ./tests/app1.cpp)
target_link_libraries(Test1 GhostConnection2 zlib)

add_executable(Test2 ./tests/app2.cpp)
target_link_libraries(Test2 GhostConnection2 zlib)