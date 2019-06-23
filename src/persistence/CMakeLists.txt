##########################################################################################################################################
############################################################ PERSISTENCE LIBRARY ##########################################################
##########################################################################################################################################

file(GLOB header_persistence_lib
	${GHOST_MODULE_ROOT_DIR}/include/ghost/persistence/SaveData.hpp
	${GHOST_MODULE_ROOT_DIR}/include/ghost/persistence/SaveData.impl.hpp
	${GHOST_MODULE_ROOT_DIR}/include/ghost/persistence/SaveManager.hpp
)

file(GLOB header_persistence_internal_lib
	${GHOST_MODULE_ROOT_DIR}/src/persistence/SaveFile.hpp
	${GHOST_MODULE_ROOT_DIR}/src/persistence/SaveData.hpp
	${GHOST_MODULE_ROOT_DIR}/src/persistence/SaveManager.hpp
)

file(GLOB source_persistence_lib
	${GHOST_MODULE_ROOT_DIR}/src/persistence/SaveFile.cpp
	${GHOST_MODULE_ROOT_DIR}/src/persistence/SaveData.cpp
	${GHOST_MODULE_ROOT_DIR}/src/persistence/SaveManager.cpp
)

source_group("API" FILES ${header_persistence_lib})

##########################################################################################################################################

add_library(ghost_persistence ${header_persistence_internal_lib}
	${header_persistence_lib}
	${source_persistence_lib}
)

target_link_libraries(ghost_persistence ${CONAN_LIBS_PROTOBUF})

if (UNIX)
	target_link_libraries(ghost_persistence pthread)
endif()

##### Unit tests #####

if ((DEFINED BUILD_TESTS) AND (${BUILD_TESTS}))
	set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/tests/protobuf/)
	set(PROTOC_SOURCE_FILE ${GHOST_MODULE_ROOT_DIR}/tests/protobuf/tests.proto)
	set(PROTOC_OUTPUT ${GHOST_MODULE_ROOT_DIR}/tests/protobuf/)

	generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})

	file(GLOB source_persistence_tests
		${GHOST_MODULE_ROOT_DIR}/tests/SaveFileTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/SaveDataTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/SaveManagerTests.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/PersistenceTestHelpers.cpp
		${GHOST_MODULE_ROOT_DIR}/tests/PersistenceTestHelpers.hpp
	)

	file(GLOB protobuf_persistence_tests
		${GHOST_MODULE_ROOT_DIR}/tests/protobuf/tests.pb.h
		${GHOST_MODULE_ROOT_DIR}/tests/protobuf/tests.pb.cc
	)

	add_executable(persistence_tests ${source_persistence_tests} ${protobuf_persistence_tests})
	target_link_libraries(persistence_tests ghost_persistence ${CONAN_LIBS_GTEST})

	gtest_add_tests(TARGET persistence_tests)

	set_property(TARGET persistence_tests PROPERTY FOLDER "tests")
endif()

##### Examples #####

if ((DEFINED BUILD_EXAMPLES) AND (${BUILD_EXAMPLES}))
	set(PROTOC_SOURCE_CUSTOM ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/)
	set(PROTOC_SOURCE_FILE ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/persistency_todo_list.proto)
	set(PROTOC_OUTPUT ${GHOST_MODULE_ROOT_DIR}/examples/protobuf/)

	generate_protoc(${PROTOC_SOURCE_CUSTOM} ${PROTOC_OUTPUT} ${PROTOC_SOURCE_FILE})

	# minimum code to create a module
	add_executable(persistence_todo_list
		${GHOST_MODULE_ROOT_DIR}/examples/persistence_todo_list.cpp
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/persistency_todo_list.pb.h
		${GHOST_MODULE_ROOT_DIR}/examples/protobuf/persistency_todo_list.pb.cc)
	target_link_libraries(persistence_todo_list ghost_module ghost_persistence)

	set_property(TARGET persistence_todo_list PROPERTY FOLDER "examples")
endif()
