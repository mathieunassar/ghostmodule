##########################################################################################################################################
######################################################### CONNECTION_EXTENSION LIBRARY #######################################################
##########################################################################################################################################

file(GLOB header_connectionextension_lib
	${GHOST_MODULE_ROOT_DIR}/include/ghost/connection_extension/ConnectionExtensionBuilder.hpp
	${GHOST_MODULE_ROOT_DIR}/include/ghost/connection_extension/ConnectionExtension.hpp
)

file(GLOB header_connectionextension_internal_lib
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/ConnectionExtensionBuilder.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/ConnectionExtension.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteControlClient.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteAccessServer.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteControllersHandler.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteConsole.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteHandler.hpp
)

file(GLOB source_connectionextension_lib
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/ConnectionExtensionBuilder.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/ConnectionExtension.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteControlClient.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteAccessServer.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteControllersHandler.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteConsole.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connection_extension/RemoteHandler.cpp
)

source_group("API" FILES ${header_connectionextension_lib})

##########################################################################################################################################

add_library(ghost_connection_extension ${header_connectionextension_internal_lib}
	${header_connectionextension_lib}
	${source_connectionextension_lib}
)

target_link_libraries(ghost_connection_extension ghost_module ghost_connection)

##### Unit tests #####

if ((DEFINED BUILD_TESTS) AND (${BUILD_TESTS}))
	file(GLOB source_connection_extension_tests
		${GHOST_MODULE_ROOT_DIR}/tests/connection_extension/ConnectionExtensionTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/connection/ConnectionTestUtils.hpp
		${GHOST_MODULE_ROOT_DIR}/tests/connection/ConnectionTestUtils.cpp
	)

	add_executable(connection_extension_tests ${source_connection_extension_tests})
	target_link_libraries(connection_extension_tests ghost_connection_extension ${CONAN_LIBS_GTEST})

	gtest_add_tests(TARGET connection_extension_tests)

	set_property(TARGET connection_extension_tests PROPERTY FOLDER "tests")
endif()

##### Examples #####

if ((DEFINED BUILD_EXAMPLES) AND (${BUILD_EXAMPLES}))
	# minimum code to create a remote module
	add_executable(connection_extension_test
		${GHOST_MODULE_ROOT_DIR}/examples/connection_extension_test.cpp)
	target_link_libraries(connection_extension_test ghost_module ghost_connection ghost_connection_grpc ghost_connection_extension)

	set_property(TARGET connection_extension_test PROPERTY FOLDER "examples")
endif()
