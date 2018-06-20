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
./include/Connection.hpp
./include/Server.hpp
./include/ClientHandler.hpp
./include/Client.hpp
./include/Publisher.hpp
./include/Subscriber.hpp
./include/Message.hpp
./include/ProtobufMessage.hpp
)

file(GLOB header_connection2_lib_internal
./include/internal/Publisher.hpp
./include/internal/Subscriber.hpp
./include/internal/GenericMessageConverter.hpp
)

file(GLOB header_connection2_lib_internal_network
./include/internal/network/ServerGRPC.hpp
./include/internal/network/ClientGRPC.hpp
./include/internal/network/RemoteClientGRPC.hpp
./include/internal/network/PublisherCoreGRPC.hpp
./include/internal/network/SubscriberCoreGRPC.hpp
)

file(GLOB source_connection2_lib_network
./src/network/ServerGRPC.cpp
./src/network/ClientGRPC.cpp
./src/network/RemoteClientGRPC.cpp
./src/network/PublisherCoreGRPC.cpp
./src/network/SubscriberCoreGRPC.cpp
)

file(GLOB protobuf_connection2_lib
./protobuf/Ghost.pb.h
./protobuf/Ghost.grpc.pb.h
./protobuf/Ghost.pb.cc
./protobuf/Ghost.grpc.pb.cc
)

file(GLOB source_connection2_lib
./src/ProtobufMessage.cpp
./src/GenericMessageConverter.cpp
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

target_link_libraries(GhostConnection2 ws2_32 libprotobuf gpr grpc grpc++ zlib grpc_unsecure grpc++_unsecure)

set_property(TARGET GhostConnection2 PROPERTY FOLDER "Libraries")

#####################################################################

# Unit tests: Primitive Serialization
file(GLOB source_connection2_tests
./tests/MessageTests.cpp)

set(libraries_connection2_tests
GhostConnection2)

add_executable(Connection_MessageTests ${source_connection2_tests})
target_link_libraries(Connection_MessageTests ${libraries_connection2_tests})
set_property(TARGET Connection_MessageTests PROPERTY FOLDER "Tests/UnitTests")
add_test(UnitTests_Connection_MessageTests Connection_MessageTests)

add_executable(Test1 ./tests/app1.cpp)
target_link_libraries(Test1 GhostConnection2 zlib)

add_executable(Test2 ./tests/app2.cpp)
target_link_libraries(Test2 GhostConnection2 zlib)