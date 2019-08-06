##########################################################################################################################################
############################################################ CONNECTION LIBRARY ##########################################################
##########################################################################################################################################

# Protobuf files generation
set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/protobuf/)
set(PROTOC_SOURCE_FILE
	${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/GenericMessage.proto
	${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/ConnectionConfiguration.proto)
set(PROTOC_OUTPUT ${GHOST_MODULE_ROOT_DIR}/protobuf/)

generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} "${PROTOC_SOURCE_FILE}")

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

file(GLOB header_connection_internal_lib
${GHOST_MODULE_ROOT_DIR}/src/connection/Connection.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Connection.impl.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/GenericMessageConverter.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ProtobufMessage.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ProtobufConfiguration.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/MessageHandler.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/MessageHandlerCallback.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionManager.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionFactory.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionFactory.impl.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionFactoryRule.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionFactoryRule.impl.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/WriterSink.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ReaderSink.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/QueuedSink.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/GenericWriter.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/GenericWriter.impl.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/GenericReader.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/GenericReader.impl.hpp
)

file(GLOB protobuf_connection_lib
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/GenericMessage.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/GenericMessage.grpc.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/GenericMessage.pb.cc
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/GenericMessage.grpc.pb.cc
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/ConnectionConfiguration.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/ConnectionConfiguration.grpc.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/ConnectionConfiguration.pb.cc
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/ConnectionConfiguration.grpc.pb.cc
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
source_group("Protobuf" FILES ${protobuf_connection_lib})

##########################################################################################################################################

add_library(ghost_connection
	${header_connection_lib}
	${header_connection_internal_lib}
	${source_connection_lib}
	${protobuf_connection_lib}
	)

if (UNIX)
	target_link_libraries(ghost_connection pthread)
endif()

target_link_libraries(ghost_connection ${CONAN_LIBS_PROTOBUF})

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
