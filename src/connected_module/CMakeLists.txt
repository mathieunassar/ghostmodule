##########################################################################################################################################
######################################################### CONNECTED_MODULE LIBRARY #######################################################
##########################################################################################################################################

file(GLOB header_connectedmodule_lib
	${GHOST_MODULE_ROOT_DIR}/include/ghost/connected_module/ConnectedModuleComponentBuilder.hpp
	${GHOST_MODULE_ROOT_DIR}/include/ghost/connected_module/ConnectedModule.hpp
)

file(GLOB header_connectedmodule_internal_lib
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/ConnectedModuleComponentBuilder.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/ConnectedModule.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteControlClient.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteAccessServer.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteControllersHandler.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteConsole.hpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteHandler.hpp
)

file(GLOB source_connectedmodule_lib
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/ConnectedModuleComponentBuilder.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/ConnectedModule.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteControlClient.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteAccessServer.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteControllersHandler.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteConsole.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteHandler.cpp
)

source_group("API" FILES ${header_connectedmodule_lib})

##########################################################################################################################################

add_library(ghost_connected_module ${header_connectedmodule_internal_lib}
	${header_connectedmodule_lib}
	${source_connectedmodule_lib}
)

target_link_libraries(ghost_connected_module ghost_module ghost_connection)

##### Unit tests #####

if ((DEFINED BUILD_TESTS) AND (${BUILD_TESTS}))
	file(GLOB source_connected_module_tests
		${GHOST_MODULE_ROOT_DIR}/tests/connected_module/ConnectedModuleTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/connection/ConnectionTestUtils.hpp
		${GHOST_MODULE_ROOT_DIR}/tests/connection/ConnectionTestUtils.cpp
	)

	add_executable(connected_module_tests ${source_connected_module_tests})
	target_link_libraries(connected_module_tests ghost_connected_module ${CONAN_LIBS_GTEST})

	gtest_add_tests(TARGET connected_module_tests)

	set_property(TARGET connected_module_tests PROPERTY FOLDER "tests")
endif()

##### Examples #####

if ((DEFINED BUILD_EXAMPLES) AND (${BUILD_EXAMPLES}))
	# minimum code to create a remote module
	add_executable(connected_module_test
		${GHOST_MODULE_ROOT_DIR}/examples/connected_module_test.cpp)
	target_link_libraries(connected_module_test ghost_module ghost_connection ghost_connection_grpc ghost_connected_module)

	set_property(TARGET connected_module_test PROPERTY FOLDER "examples")
endif()
