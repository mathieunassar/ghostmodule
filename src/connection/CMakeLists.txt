##########################################################################################################################################
############################################################ CONNECTION LIBRARY ##########################################################
##########################################################################################################################################

# Protobuf files generation
set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/protobuf/)
set(PROTOC_SOURCE_FILE
	${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/GenericMessage.proto
	${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/ConnectionConfiguration.proto)

generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} "${PROTOC_SOURCE_FILE}")

# targets defintion
file(GLOB header_connection_lib
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Connection.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/ReadableConnection.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/WritableConnection.hpp
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
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/WriterSink.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/Reader.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/ReaderSink.hpp
)

file(GLOB header_connection_lib_internal
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ConnectionFactoryRule.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/MessageHandlerCallback.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/GenericWriter.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/GenericReader.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/GenericMessageConverter.hpp
${GHOST_MODULE_ROOT_DIR}/include/ghost/connection/internal/ProtobufMessage.hpp
)

file(GLOB header_connection_internal_lib
${GHOST_MODULE_ROOT_DIR}/src/connection/Configuration.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/MessageHandler.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionManager.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionFactory.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Writer.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Reader.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/WriterSink.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ReaderSink.hpp
${GHOST_MODULE_ROOT_DIR}/src/connection/QueuedSink.hpp
)

file(GLOB protobuf_connection_lib
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/GenericMessage.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/GenericMessage.pb.cc
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/ConnectionConfiguration.pb.h
${GHOST_MODULE_ROOT_DIR}/protobuf/ghost/connection/ConnectionConfiguration.pb.cc
)

file(GLOB source_connection_lib
${GHOST_MODULE_ROOT_DIR}/src/connection/ProtobufMessage.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/MessageHandler.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/GenericMessageConverter.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Configuration.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionConfiguration.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/NetworkConnectionConfiguration.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionManager.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionFactory.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ConnectionFactoryRule.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/WritableConnection.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/WriterSink.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Writer.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ReadableConnection.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/Reader.cpp
${GHOST_MODULE_ROOT_DIR}/src/connection/ReaderSink.cpp
)

source_group("API" FILES ${header_connection_lib})
source_group("Protobuf" FILES ${protobuf_connection_lib})
source_group("API\\internal" FILES ${header_connection_lib_internal})

##########################################################################################################################################

add_library(ghost_connection
	${header_connection_lib}
	${header_connection_lib_internal}
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
		${GHOST_MODULE_ROOT_DIR}/tests/ConnectionTestUtils.hpp
		${GHOST_MODULE_ROOT_DIR}/tests/ConnectionTestUtils.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/MessageTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/ConfigurationTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/ReaderWriterTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/ConnectionTests.cpp)

	add_executable(connection_tests ${source_connection_tests})
	target_link_libraries(connection_tests ghost_connection ${CONAN_LIBS_GTEST})
	
	gtest_add_tests(TARGET connection_tests)

	set_property(TARGET connection_tests PROPERTY FOLDER "tests")
endif()
