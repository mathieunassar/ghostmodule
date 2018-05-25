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
./include/Client.hpp
./include/Publisher.hpp
./include/Subscriber.hpp
)

file(GLOB header_connection2_lib_internal
./include/internal/Connection.hpp
./include/internal/Server.hpp
./include/internal/Client.hpp
./include/internal/Publisher.hpp
./include/internal/Subscriber.hpp
)

file(GLOB header_connection2_lib_internal_network
./include/internal/network/ServerCoreGRPC.hpp
./include/internal/network/ClientCoreGRPC.hpp
./include/internal/network/PublisherCoreGRPC.hpp
./include/internal/network/SubscriberCoreGRPC.hpp
)

file(GLOB source_connection2_lib_network
./src/network/ServerCoreGRPC.cpp
./src/network/ClientCoreGRPC.cpp
./src/network/PublisherCoreGRPC.cpp
./src/network/SubscriberCoreGRPC.cpp)

file(GLOB protobuf_connection2_lib
./protobuf/Ghost.pb.h
./protobuf/Ghost.grpc.pb.h
./protobuf/Ghost.pb.cc
./protobuf/Ghost.grpc.pb.cc
)

file(GLOB source_connection2_lib
./src/main.cpp)

source_group("Header Files\\internal" FILES ${header_connection2_lib_internal})
source_group("Header Files\\internal\\network" FILES ${header_connection2_lib_internal_network})
source_group("Source Files\\network" FILES ${source_connection2_lib_network})
source_group("Protobuf" FILES ${protobuf_connection2_lib})

#####################################################################

add_executable(GhostConnection2 ${header_connection2_lib}
	${header_connection2_lib_internal}
	${header_connection2_lib_internal_network}
	${source_connection2_lib_network}
	${source_connection2_lib}
	${protobuf_connection2_lib}
	)

target_link_libraries(GhostConnection2 libprotobuf grpc grpc++)

set_property(TARGET GhostConnection2 PROPERTY FOLDER "Libraries")
