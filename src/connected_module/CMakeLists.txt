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
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteController.hpp
)

file(GLOB source_connectedmodule_lib
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/ConnectedModuleComponentBuilder.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/ConnectedModule.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteControlClient.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteAccessServer.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteControllersHandler.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteConsole.cpp
	${GHOST_MODULE_ROOT_DIR}/src/connected_module/RemoteController.cpp
)

source_group("API" FILES ${header_connectedmodule_lib})

##########################################################################################################################################

add_library(ghost_connected_module ${header_connectedmodule_internal_lib}
	${header_connectedmodule_lib}
	${source_connectedmodule_lib}
)

target_link_libraries(ghost_connected_module ghost_module ghost_connection)

##### Unit tests #####

#if ((DEFINED BUILD_TESTS) AND (${BUILD_TESTS}))
#	set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/tests/protobuf/)
#	set(PROTOC_SOURCE_FILE ${GHOST_MODULE_ROOT_DIR}/tests/protobuf/tests.proto)
#	set(PROTOC_OUTPUT ${GHOST_MODULE_ROOT_DIR}/tests/protobuf/)
#
#	generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})
#
#	file(GLOB source_persistence_tests
#		${GHOST_MODULE_ROOT_DIR}/tests/persistence/SaveFileTests.cpp
#		${GHOST_MODULE_ROOT_DIR}/tests/persistence/SaveDataTests.cpp
#		${GHOST_MODULE_ROOT_DIR}/tests/persistence/SaveManagerTests.cpp
#		${GHOST_MODULE_ROOT_DIR}/tests/persistence/PersistenceTestHelpers.cpp
#		${GHOST_MODULE_ROOT_DIR}/tests/persistence/PersistenceTestHelpers.hpp
#	)
#
#	file(GLOB protobuf_persistence_tests
#		${GHOST_MODULE_ROOT_DIR}/tests/protobuf/tests.pb.h
#		${GHOST_MODULE_ROOT_DIR}/tests/protobuf/tests.pb.cc
#	)
#
#	add_executable(persistence_tests ${source_persistence_tests} ${protobuf_persistence_tests})
#	target_link_libraries(persistence_tests ghost_persistence ${CONAN_LIBS_GTEST})
#
#	gtest_add_tests(TARGET persistence_tests)
#
#	set_property(TARGET persistence_tests PROPERTY FOLDER "tests")
#endif()

##### Examples #####

if ((DEFINED BUILD_EXAMPLES) AND (${BUILD_EXAMPLES}))
	# minimum code to create a remote module
	add_executable(connected_module_test
		${GHOST_MODULE_ROOT_DIR}/examples/connected_module_test.cpp)
	target_link_libraries(connected_module_test ghost_module ghost_connection ghost_connection_grpc ghost_connected_module)

	set_property(TARGET connected_module_test PROPERTY FOLDER "examples")
endif()
